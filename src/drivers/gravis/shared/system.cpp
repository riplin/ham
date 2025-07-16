//Copyright 2025-Present riplin

#include <support/env.h>
#include <ham/drivers/gravis/shared/system.h>
#include <ham/drivers/gravis/shared/gf1/page.h>
#include <ham/drivers/gravis/shared/gf1/dramio.h>
#include <ham/drivers/gravis/shared/gf1/intrstat.h>
#include <ham/drivers/gravis/shared/midi/control.h>
#include <ham/drivers/gravis/shared/gf1/voice/endloc.h>
#include <ham/drivers/gravis/shared/gf1/voice/panpos.h>
#include <ham/drivers/gravis/shared/gf1/voice/curloc.h>
#include <ham/drivers/gravis/shared/gf1/voice/volrmp.h>
#include <ham/drivers/gravis/shared/gf1/voice/curvol.h>
#include <ham/drivers/gravis/shared/gf1/global/reset.h>
#include <ham/drivers/gravis/shared/gf1/voice/vocctrl.h>
#include <ham/drivers/gravis/shared/gf1/voice/frqctrl.h>
#include <ham/drivers/gravis/shared/gf1/voice/actvvoc.h>
#include <ham/drivers/gravis/shared/gf1/voice/intrsrc.h>
#include <ham/drivers/gravis/shared/gf1/voice/volrmprt.h>
#include <ham/drivers/gravis/shared/gf1/voice/startloc.h>
#include <ham/drivers/gravis/shared/gf1/global/dramdma.h>
#include <ham/drivers/gravis/shared/gf1/global/tmrctrl.h>
#include <ham/drivers/gravis/shared/gf1/global/smpctrl.h>
#include <ham/drivers/gravis/shared/gf1/global/dramioad.h>

namespace Ham::Gravis::Shared::Function::System
{

namespace InitializeError
{

    const char* ToString(InitializeError_t error)
    {
        switch (error)
        {
        case Success:
            return "Success";
        case UltrasoundEnvNotFound:
            return "ULTRASND environment variable not found";
        case UltrasoundEnvMalformed:
            return "ULTRASND environment variable malformed; expected ULTRASND=AAA,B,C,D,E";
        case UltrasoundEnvInvalidValues:
            return "ULTRASND environment variable contains invalid values";
        case CardNotFound:
            return "Card not found at base address found in ULTRASND environment variable";
        default:
            return "Unknown error";
        }
    }
}

GF1::DramIO_t ReadMemory(Register_t baseAddress, GF1::Global::DramIOAddress_t address)
{
    SYS_ClearInterrupts();
    GF1::Global::DramIOAddress::Write(baseAddress, address);
    GF1::DramIO_t ret = GF1::DramIO::Read(baseAddress);
    SYS_RestoreInterrupts();
    return ret;
}

void WriteMemory(Register_t baseAddress, GF1::Global::DramIOAddress_t address, GF1::DramIO_t value)
{
    SYS_ClearInterrupts();
    GF1::Global::DramIOAddress::Write(baseAddress, address);
    GF1::DramIO::Write(baseAddress, value);
    SYS_RestoreInterrupts();
}

void Wait(Register_t baseAddress, uint16_t count = 1)
{
    using namespace Ham::Gravis::Shared::GF1;

    for (uint32_t i = 0; i < 7 * count; i++)
        DramIO::Read(baseAddress);
}

bool DetectCard(Register_t baseAddress)
{
    using namespace Ham::Gravis::Shared::GF1;
    using namespace Ham::Gravis::Shared::GF1::Global;

    //Reset card.
    Reset::Write(baseAddress, Reset::MasterDisable);

    //Wait for card to properly reset.
    Wait(baseAddress, 2);

    //Re-enable the card.
    Reset::Write(baseAddress, Reset::MasterEnable);

    //Wait for card to stabilize.
    Wait(baseAddress, 2);

    //Store original values.
    DramIO_t value1 = ReadMemory(baseAddress, 0);
    DramIO_t value2 = ReadMemory(baseAddress, 1);

    //Write test pattern.
    WriteMemory(baseAddress, 0, 0x55);
    WriteMemory(baseAddress, 1, 0xaa);

    //Read back test pattern.
    DramIO_t check1 = ReadMemory(baseAddress, 0);
    DramIO_t check2 = ReadMemory(baseAddress, 1);

    //Restore original values.
    WriteMemory(baseAddress, 0, value1);
    WriteMemory(baseAddress, 1, value2);

    //If they are the same, success!
    return (check1 == 0x55) && (check2 == 0xaa);
}

void ResetCard(Register_t baseAddress, uint8_t voiceCount)
{
    using namespace Has;
    using namespace Ham::Gravis::Shared;
    using namespace Ham::Gravis::Shared::GF1;
    using namespace Ham::Gravis::Shared::GF1::Global;
    using namespace Ham::Gravis::Shared::GF1::Voice;

    voiceCount = min<uint8_t>(max<uint8_t>(voiceCount, 14), 32);

    //Set first two bytes of memory to 0 for quiet voices.
    WriteMemory(baseAddress, 0, 0);
    WriteMemory(baseAddress, 1, 0);

    SYS_ClearInterrupts();

    //Reset the card.
    Reset::Write(baseAddress, Reset::MasterDisable);

    //Wait.
    Wait(baseAddress, 10);

    //Re-enable the card (silent).
    Reset::Write(baseAddress, Reset::MasterEnable | Reset::DacDisable | Reset::InterruptDisable);

    //Wait.
    Wait(baseAddress, 10);

    //Reset MIDI.
    Midi::Control::Write(baseAddress, Midi::Control::Reset);

    //Wait.
    Wait(baseAddress, 10);

    //Re-enable MIDI.
    Midi::Control::Write(baseAddress, Midi::Control::Enable);

    //Clear interrupts.
    DramDmaControl::Write(baseAddress, DramDmaControl::DmaInterruptDisable);
    TimerControl::Write(baseAddress, TimerControl::Timer1Disable | TimerControl::Timer2Disable);
    SamplingControl::Write(baseAddress, SamplingControl::DmaInterruptDisable);

    //Set number of active voices.
    ActiveVoices::Write(baseAddress, voiceCount - 1);

    //Clear pending interrupts.
    InterruptStatus::Read(baseAddress);
    DramDmaControl::Read(baseAddress);
    SamplingControl::Read(baseAddress);
    InterruptSource::Read(baseAddress);

    for (uint8_t voice = 0; voice < voiceCount; ++voice)
    {
        //Select voice.
        Page::Write(baseAddress, voice);

        //Reset voice registers to default state.
        VoiceControl::Write(baseAddress, VoiceControl::Stop | VoiceControl::Stopped);
        FrequencyControl::Write(baseAddress, 0x400);//1.0
        StartLocation::Write(baseAddress, 0);
        EndLocation::Write(baseAddress, 0);
        VolumeRampRate::Write(baseAddress, 1);
        VolumeRamp::WriteStart(baseAddress, 0x10);
        VolumeRamp::WriteEnd(baseAddress, 0xe0);
        CurrentVolume::Write(baseAddress, 0);
        CurrentLocation::Write(baseAddress, 0);
        PanPosition::Write(baseAddress, 7);
    }

    //Clear pending interrupts again.
    InterruptStatus::Read(baseAddress);
    DramDmaControl::Read(baseAddress);
    SamplingControl::Read(baseAddress);
    InterruptSource::Read(baseAddress);

    //Enable DAC, enable interrupts.
    Reset::Write(baseAddress, Reset::MasterEnable | Reset::DacEnable | Reset::InterruptEnable);

    SYS_RestoreInterrupts();
}

static Has::IAllocator* s_Allocator = nullptr;
Register_t s_BaseAddress = 0;
uint8_t s_PlayDMA = 0;
uint8_t s_RecordDMA = 0;
uint8_t s_UltrasoundInterrupt = 0;
uint8_t s_SoundBlasterInterrupt = 0;

InitializeError_t ParseEnvironmentVariable()
{
    const char* ultrasnd = Support::GetEnv("ULTRASND");
    if (ultrasnd != nullptr)
    {
        const char* ptr = ultrasnd;
        int delims = 0;
        while (*ptr != '\0')
        {
            if (*ptr == ',')
                ++delims;
            ++ptr;
        }

        if (delims != 4)
        {
            return InitializeError::UltrasoundEnvMalformed;
        }

        int values[5] = { 0 };
        int idx = 0;
        int val = 0;
        ptr = ultrasnd;
        while(*ptr != '\0')
        {
            if (*ptr == ',')
            {
                values[idx] = val;
                ++idx;
                val = 0;
            }

            if ((*ptr >= '0') && (*ptr <= '9'))
            {
                if (idx == 0)
                {
                    //Value is HEX, but no 'ABCDEF' characters are used.
                    val = val * 16 + (*ptr - '0');
                }
                else
                {
                    //dma / interrupt are decimal
                    val = val * 10 + (*ptr - '0');
                }
            }
            ++ptr;
        }
        values[idx] = val;

        if ((values[0] != 0x220) &&
            (values[0] != 0x240) &&
            (values[0] != 0x250) &&
            (values[0] != 0x260))
        {
            return InitializeError::UltrasoundEnvInvalidValues;
        }

        if ((values[1] != 1) &&
            (values[1] != 3) &&
            (values[1] != 5) &&
            (values[1] != 6) &&
            (values[1] != 7))
        {
            return InitializeError::UltrasoundEnvInvalidValues;
        }

        if ((values[2] != 1) &&
            (values[2] != 3) &&
            (values[2] != 5) &&
            (values[2] != 6) &&
            (values[2] != 7))
        {
            return InitializeError::UltrasoundEnvInvalidValues;
        }

        if ((values[3] != 5) &&
            (values[3] != 7) &&
            (values[3] != 11) &&
            (values[3] != 12) &&
            (values[3] != 15))
        {
            return InitializeError::UltrasoundEnvInvalidValues;
        }

        if ((values[4] != 5) &&
            (values[4] != 7))
        {
            return InitializeError::UltrasoundEnvInvalidValues;
        }

        s_BaseAddress = values[0];
        s_PlayDMA = values[1];
        s_RecordDMA = values[2];
        s_UltrasoundInterrupt = values[3];
        s_SoundBlasterInterrupt = values[4];

        return InitializeError::Success;
    }

    return InitializeError::UltrasoundEnvNotFound;
}

InitializeError_t Initialize(Has::IAllocator& allocator)
{
    s_Allocator = &allocator;

    InitializeError_t ret = ParseEnvironmentVariable();

    if (ret == InitializeError::Success)
    {
        if (DetectCard(s_BaseAddress))
        {
            ResetCard(s_BaseAddress, 14);
        }
        else
        {
            ret = InitializeError::CardNotFound;
        }
    }

    return ret;
}

void Shutdown()
{

}

}