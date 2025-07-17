//Copyright 2025-Present riplin

#include <support/env.h>
#include <has/system/pic.h>
#include <has/testing/log.h>
#include <has/system/interrup.h>
#include <ham/drivers/gravis/shared/system.h>
#include <ham/drivers/gravis/shared/gf1/page.h>
#include <ham/drivers/gravis/shared/midi/data.h>
#include <ham/drivers/gravis/shared/gf1/dramio.h>
#include <ham/drivers/gravis/shared/midi/status.h>
#include <ham/drivers/gravis/shared/gf1/tmrdata.h>
#include <ham/drivers/gravis/shared/gf1/tmrctrl.h>
#include <ham/drivers/gravis/shared/gf1/mixcrtl.h>
#include <ham/drivers/gravis/shared/gf1/dmactrl.h>
#include <ham/drivers/gravis/shared/gf1/regctrl.h>
#include <ham/drivers/gravis/shared/gf1/intrctrl.h>
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
#include <ham/drivers/gravis/shared/gf1/global/tmrcnt.h>
#include <ham/drivers/gravis/shared/gf1/voice/volrmpct.h>
#include <ham/drivers/gravis/shared/gf1/voice/volrmprt.h>
#include <ham/drivers/gravis/shared/gf1/voice/startloc.h>
#include <ham/drivers/gravis/shared/gf1/global/dramdma.h>
#include <ham/drivers/gravis/shared/gf1/global/tmrctrl.h>
#include <ham/drivers/gravis/shared/gf1/global/smpctrl.h>
#include <ham/drivers/gravis/shared/gf1/global/dramioad.h>

extern volatile uint32_t s_Counter;

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
        case AlreadyInitialized:
            return "Already initialized";
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
    GF1::Global::DramIOAddress::Write(baseAddress, address);
    GF1::DramIO_t ret = GF1::DramIO::Read(baseAddress);
    return ret;
}

void WriteMemory(Register_t baseAddress, GF1::Global::DramIOAddress_t address, GF1::DramIO_t value)
{
    GF1::Global::DramIOAddress::Write(baseAddress, address);
    GF1::DramIO::Write(baseAddress, value);
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

    SYS_ClearInterrupts();

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

    SYS_RestoreInterrupts();

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

    SYS_ClearInterrupts();

    //Set first two bytes of memory to 0 for quiet voices.
    WriteMemory(baseAddress, 0, 0);
    WriteMemory(baseAddress, 1, 0);

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
    GF1::Global::TimerControl::Write(baseAddress, GF1::Global::TimerControl::Timer1Disable | GF1::Global::TimerControl::Timer2Disable);
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

    LOG("Gravis", "Card reset");
}

static Has::IAllocator* s_Allocator = nullptr;
Register_t s_BaseAddress = 0;
uint8_t s_PlayDMA = 0;
uint8_t s_RecordDMA = 0;
uint8_t s_UltrasoundInterrupt = 0;
uint8_t s_MidiInterrupt = 0;

static GF1::InterruptControl_t s_InterruptLatchValues[16] =
{
    0,
    0,
    GF1::InterruptControl::Gf1Interrupt2,
    GF1::InterruptControl::Gf1Interrupt3,
    0,
    GF1::InterruptControl::Gf1Interrupt5,
    0,
    GF1::InterruptControl::Gf1Interrupt7,
    0,
    0,
    0,
    GF1::InterruptControl::Gf1Interrupt11,
    GF1::InterruptControl::Gf1Interrupt12,
    0,
    0,
    GF1::InterruptControl::Gf1Interrupt15
};

static GF1::DmaControl_t s_DmaLatchValues[8] =
{
    GF1::DmaControl::Dma1SelectNone,
    GF1::DmaControl::Dma1Select1,
    GF1::DmaControl::Dma1SelectNone,
    GF1::DmaControl::Dma1Select3,
    GF1::DmaControl::Dma1SelectNone,
    GF1::DmaControl::Dma1Select5,
    GF1::DmaControl::Dma1Select6,
    GF1::DmaControl::Dma1Select7
};

void InitializeCard(Register_t baseAddress, uint8_t playDma, uint8_t recordDma, uint8_t gf1Interrupt, uint8_t midiInterrupt)
{
    using namespace Has::System;

    s_BaseAddress = baseAddress;
    s_PlayDMA = playDma;
    s_RecordDMA = recordDma;
    s_UltrasoundInterrupt = gf1Interrupt;
    s_MidiInterrupt = midiInterrupt;

    SYS_ClearInterrupts();
    GF1::MixControl_t mixControl = GF1::MixControl::LineInputDisable | GF1::MixControl::LineOutputDisable |
                                   GF1::MixControl::MicrophoneInputDisable | GF1::MixControl::LatchesEnable;

    GF1::MixControl::Write(s_BaseAddress, mixControl);

    GF1::InterruptControl_t interruptControl = s_InterruptLatchValues[gf1Interrupt & 0xf];
    GF1::InterruptControl_t midiInterruptLatch = s_InterruptLatchValues[midiInterrupt & 0xf] << GF1::InterruptControl::Shift::MidiInterruptSelector;

    GF1::DmaControl_t dmaControl = s_DmaLatchValues[playDma];
    GF1::DmaControl_t recordDmaLatch = s_DmaLatchValues[recordDma] << GF1::DmaControl::Shift::Dma2Selector;

    if ((gf1Interrupt == midiInterrupt) && (gf1Interrupt != 0))
    {
        interruptControl |= GF1::InterruptControl::CombineInterrupts;
    }
    else
    {
        interruptControl |= midiInterruptLatch;
    }

	if ((playDma == recordDma) && (playDma != 0))
    {
        dmaControl |= GF1::DmaControl::CombineDma;
    }
    else
    {
        dmaControl |= recordDmaLatch;
    }

    //Setup
    GF1::RegisterControl::Write(s_BaseAddress, GF1::RegisterControl::ClearInterruptPort);
    GF1::MixControl::Write(s_BaseAddress, mixControl | GF1::MixControl::SelectDma);
    GF1::DmaControl::Write(s_BaseAddress, 0);
    GF1::RegisterControl::Write(s_BaseAddress, GF1::RegisterControl::Default);

    //Configure DMA.
    GF1::MixControl::Write(s_BaseAddress, mixControl | GF1::MixControl::SelectDma);
    GF1::DmaControl::Write(s_BaseAddress, dmaControl | 0x80);//Undocumented flag

    //Configure Interrupts.
    GF1::MixControl::Write(s_BaseAddress, mixControl | GF1::MixControl::SelectInterrupt);
    GF1::InterruptControl::Write(s_BaseAddress, interruptControl);

    //Configure DMA again.
    GF1::MixControl::Write(s_BaseAddress, mixControl | GF1::MixControl::SelectDma);
    GF1::DmaControl::Write(s_BaseAddress, dmaControl);

    //Configure Interrupts again.
    GF1::MixControl::Write(s_BaseAddress, mixControl | GF1::MixControl::SelectInterrupt);
    GF1::InterruptControl::Write(s_BaseAddress, interruptControl);

    //Poke a port to lock the dma / interrupt port.
    GF1::Page::Write(s_BaseAddress, 0x00);

    //Set output, disable inputs.
    GF1::MixControl::Write(s_BaseAddress, 
        GF1::MixControl::LineInputDisable |
        GF1::MixControl::LineOutputEnable |
        GF1::MixControl::MicrophoneInputDisable |
        GF1::MixControl::LatchesEnable);

    //We touched MixControl, lock the dma / interrupt port by poking a different port.
    GF1::Page::Write(s_BaseAddress, 0x00);

    //Unmask the system interrupts
    if (gf1Interrupt != 0)
        PIC::Mask::UnmaskInterrupt(gf1Interrupt);

    if ((midiInterrupt != gf1Interrupt) && (midiInterrupt != 0))
        PIC::Mask::UnmaskInterrupt(midiInterrupt);
    
	//Unmask cascade interrupt 2
    if (gf1Interrupt > 7 || midiInterrupt > 7)
        PIC::Mask::UnmaskInterrupt(2);

    SYS_RestoreInterrupts();

    LOG("Gravis", "Card initialized");
}

void MaskInterrupts(uint8_t gf1Interrupt, uint8_t midiInterrupt)
{
    using namespace Has::System;

    SYS_ClearInterrupts();
    if ((gf1Interrupt != 0) && (gf1Interrupt != 2))
        PIC::Mask::MaskInterrupt(gf1Interrupt);

    if ((midiInterrupt != 0) && (midiInterrupt != 2))
        PIC::Mask::MaskInterrupt(midiInterrupt);
    SYS_RestoreInterrupts();

    LOG("Gravis", "Interrupts masked");
}

InitializeError_t ParseEnvironmentVariable(Register_t& baseAddress, uint8_t& playDma, uint8_t& recordDma, uint8_t& gf1Interrupt, uint8_t& midiInterrupt)
{
    baseAddress = 0;
    playDma = 0;
    recordDma = 0;
    gf1Interrupt = 0;
    midiInterrupt = 0;

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

        baseAddress = values[0];
        playDma = values[1];
        recordDma = values[2];
        gf1Interrupt = values[3];
        midiInterrupt = values[4];

        return InitializeError::Success;
    }

    return InitializeError::UltrasoundEnvNotFound;
}

bool SetupTimer1(uint8_t ticksPerSecond)
{
    uint16_t interval = 12500 / ticksPerSecond;
    if (interval > 256)
    {
        LOG("Gravis", "Error setting up timer 1, can't count up from %i, ticks per second: %i", interval, ticksPerSecond);
        return false;
    }

    int16_t countStart = 256 - interval;
    SYS_ClearInterrupts();
    GF1::Global::TimerCount::Write1(s_BaseAddress, GF1::Global::TimerCount_t(countStart));
    GF1::Global::TimerControl::Write(s_BaseAddress,
        GF1::Global::TimerControl::Read(s_BaseAddress) |
        GF1::Global::TimerControl::Timer1Enable);
    SYS_RestoreInterrupts();

    LOG("Gravis", "Timer 1 set up: %i ticks per second, count up from: %i", ticksPerSecond, countStart);

    return true;
}

void StartTimer1()
{
    SYS_ClearInterrupts();
    GF1::TimerControl::Write(s_BaseAddress, GF1::TimerControl::TimerControlSelect);
    GF1::TimerData::Write(s_BaseAddress, GF1::TimerData::Timer1Start | GF1::TimerData::Timer2Mask);
    SYS_RestoreInterrupts();
}

bool SetupTimer2(uint8_t ticksPerSecond)
{
    uint16_t interval = 3125 / ticksPerSecond;
    if (interval > 256)
    {
        LOG("Gravis", "Error setting up timer 2, can't count up from %i, ticks per second: %i", interval, ticksPerSecond);
        return false;
    }

    int16_t countStart = 256 - interval;
    SYS_ClearInterrupts();
    GF1::Global::TimerCount::Write2(s_BaseAddress, GF1::Global::TimerCount_t(countStart));
    GF1::Global::TimerControl::Write(s_BaseAddress,
        GF1::Global::TimerControl::Read(s_BaseAddress) |
        GF1::Global::TimerControl::Timer2Enable);
    SYS_RestoreInterrupts();

    LOG("Gravis", "Timer 2 set up: %i ticks per second, count up from: %i", ticksPerSecond, countStart);

    return true;
}

void StartTimer2()
{
    SYS_ClearInterrupts();
    GF1::TimerControl::Write(s_BaseAddress, GF1::TimerControl::TimerControlSelect);
    GF1::TimerData::Write(s_BaseAddress, GF1::TimerData::Timer2Start | GF1::TimerData::Timer1Mask);
    SYS_RestoreInterrupts();
}

void ShutdownTimers()
{
    SYS_ClearInterrupts();
    GF1::Global::TimerControl::Write(s_BaseAddress, 0);
    GF1::TimerControl::Write(s_BaseAddress, GF1::TimerControl::TimerControlSelect);
    GF1::TimerData::Write(s_BaseAddress, 0);
    GF1::TimerControl::Write(s_BaseAddress, GF1::TimerControl::TimerControlSelect);
    GF1::TimerData::Write(s_BaseAddress, GF1::TimerData::ClearTimerInterrupt);
    SYS_RestoreInterrupts();

    LOG("Gravis", "Timers shut down");
}

MidiTransmitCallback_t s_MidiTransmitCallback = nullptr;
MidiReceiveCallback_t s_MidiReceiveCallback = nullptr;
TimerCallback_t s_Timer1Callback = nullptr;
TimerCallback_t s_Timer2Callback = nullptr;

void MidiInterruptHandler()
{
    using namespace Has::System;

    PIC::Control::ClearPendingInterrupt(s_MidiInterrupt);

    Midi::Status_t midiStatus = Midi::Status::Read(s_BaseAddress);
    if ((midiStatus & Midi::Status::Transmit) != 0)
    {
        if (s_MidiTransmitCallback != nullptr)
            s_MidiTransmitCallback(midiStatus);
    }

    if ((midiStatus & Midi::Status::Receive) != 0)
    {
        Midi::Data_t midiData = Midi::Data::Read(s_BaseAddress);
        if (s_MidiReceiveCallback != nullptr)
            s_MidiReceiveCallback(midiStatus, midiData);
    }
}

void UltrasoundInterruptHandler()
{
    using namespace Has::System;

    PIC::Control::ClearPendingInterrupt(s_UltrasoundInterrupt);

    s_Counter = s_Counter + 1;
    do
    {
        GF1::InterruptStatus_t interruptStatus = GF1::InterruptStatus::Read(s_BaseAddress);

        if (interruptStatus == 0)
            break;

        // Handle DMA interrupts
        if ((interruptStatus & GF1::InterruptStatus::DmaTC))
        {
            SYS_ClearInterrupts();
            GF1::Global::DramDmaControl_t dramDmaControl = GF1::Global::DramDmaControl::Read(s_BaseAddress);
            SYS_RestoreInterrupts();

            if ((dramDmaControl & GF1::Global::DramDmaControl::DmaInterruptState) != 0)
            {
                //TODO: handle dma.
            }

            SYS_ClearInterrupts();
            GF1::Global::SamplingControl_t samplingControl = GF1::Global::SamplingControl::Read(s_BaseAddress);
            SYS_RestoreInterrupts();

            if ((samplingControl & GF1::Global::SamplingControl::DmaInterruptState) != 0)
            {
                //TODO: handle dma.
            }
        }

        // Handle MIDI interrupts
        if ((interruptStatus & (GF1::InterruptStatus::MidiReceive | GF1::InterruptStatus::MidiTransmit)) != 0)
        {
            Midi::Status_t midiStatus = Midi::Status::Read(s_BaseAddress);
            if ((interruptStatus & GF1::InterruptStatus::MidiTransmit) != 0)
            {
                if (s_MidiTransmitCallback != nullptr)
                    s_MidiTransmitCallback(midiStatus);
            }

            if ((interruptStatus & GF1::InterruptStatus::MidiReceive) != 0)
            {
                Midi::Data_t midiData = Midi::Data::Read(s_BaseAddress);
                if (s_MidiReceiveCallback != nullptr)
                    s_MidiReceiveCallback(midiStatus, midiData);
            }
        }

        // Handle Timer 1 interrupt
        if ((interruptStatus & GF1::InterruptStatus::Timer1) != 0)
        {
            SYS_ClearInterrupts();

            // Toggle the timer to restart.
            GF1::Global::TimerControl_t timerControl = GF1::Global::TimerControl::Read(s_BaseAddress);
            GF1::Global::TimerControl::Write(s_BaseAddress, timerControl & ~GF1::Global::TimerControl::Timer1);
            GF1::Global::TimerControl::Write(s_BaseAddress, timerControl);

            if (s_Timer1Callback != nullptr)
                s_Timer1Callback();

            SYS_RestoreInterrupts();
        }

        // Handle Timer 2 interrupt
        if ((interruptStatus & GF1::InterruptStatus::Timer2) != 0)
        {
            SYS_ClearInterrupts();

            // Toggle the timer to restart.
            GF1::Global::TimerControl_t timerControl = GF1::Global::TimerControl::Read(s_BaseAddress);
            GF1::Global::TimerControl::Write(s_BaseAddress, timerControl & ~GF1::Global::TimerControl::Timer2);
            GF1::Global::TimerControl::Write(s_BaseAddress, timerControl);

            if (s_Timer2Callback != nullptr)
                s_Timer2Callback();

            SYS_RestoreInterrupts();
        }
        
        // We only service a voice once for a particular reason.
        uint32_t waveFormsServiced = 0;
        uint32_t volumeRampsServiced = 0;

        // Handle Voice interrupts
        if ((interruptStatus & (GF1::InterruptStatus::WaveTable | GF1::InterruptStatus::VolumeRamp)) != 0) do
        {
            GF1::Voice::InterruptSource_t interruptSource = GF1::Voice::InterruptSource::Read(s_BaseAddress);

            // If both bits are set, then we are done.
            if ((interruptSource & (GF1::Voice::InterruptSource::VolumeRamp | GF1::Voice::InterruptSource::WaveTable)) ==
                (GF1::Voice::InterruptSource::VolumeRamp | GF1::Voice::InterruptSource::WaveTable))
                break;

            uint8_t voice = interruptSource & GF1::Voice::InterruptSource::Voice;

            uint32_t voiceMask = 1 << voice;
            if (((interruptSource & GF1::Voice::InterruptSource::WaveTable) == 0) &&
                ((waveFormsServiced & voiceMask) == 0))
            {
                waveFormsServiced |= voiceMask;

                SYS_ClearInterrupts();

                // Select the voice.
                GF1::Page::Write(s_BaseAddress, voice);

                GF1::Voice::VoiceControl_t voiceControl = GF1::Voice::VoiceControl::Read(s_BaseAddress);
                GF1::Voice::VolumeRampControl_t volumeRampControl = GF1::Voice::VolumeRampControl::Read(s_BaseAddress);
                
                //Not looping, no roll over condition
                if (((voiceControl & GF1::Voice::VoiceControl::LoopToBegin) == 0) &&
                    ((volumeRampControl & GF1::Voice::VolumeRampControl::RollOverCondition) == 0))
                {
                    // Disable interrupt on voice.
                    voiceControl &= ~GF1::Voice::VoiceControl::InterruptControl;
                    
                    // Stop the voice.
                    voiceControl |= GF1::Voice::VoiceControl::Stop | GF1::Voice::VoiceControl::Stopped;

                    // Submit.
                    GF1::Voice::VoiceControl::Write(s_BaseAddress, voiceControl);
                    // Docs say to write this reg twice with a delay to make sure it sticks.
                    Wait(s_BaseAddress);
                    GF1::Voice::VoiceControl::Write(s_BaseAddress, voiceControl);
                }
                SYS_RestoreInterrupts();

                //TODO: add user callback.
            }
            
            if (((interruptSource & GF1::Voice::InterruptSource::VolumeRamp) == 0) &&
                ((volumeRampsServiced & voiceMask) == 0))
            {
                volumeRampsServiced |= voiceMask;

                SYS_ClearInterrupts();

                // Select the voice.
                GF1::Page::Write(s_BaseAddress, voice);

                GF1::Voice::VolumeRampControl_t volumeRampControl = GF1::Voice::VolumeRampControl::Read(s_BaseAddress);

                if ((volumeRampControl & GF1::Voice::VolumeRampControl::Looping) == 0)
                {
                    //Stop the ramp.
                    volumeRampControl |= GF1::Voice::VolumeRampControl::Stop | GF1::Voice::VolumeRampControl::Stopped;

                    // Submit.
                    GF1::Voice::VolumeRampControl::Write(s_BaseAddress, volumeRampControl);
                    // Docs say to write this reg twice with a delay to make sure it sticks.
                    Wait(s_BaseAddress);
                    GF1::Voice::VolumeRampControl::Write(s_BaseAddress, volumeRampControl);
                }
                SYS_RestoreInterrupts();

                //TODO: add user callback.
            }

        } while (true);
    } while (true);
}

void SetMidiTransmitCallback(const MidiTransmitCallback_t& callback)
{
    SYS_ClearInterrupts();
    s_MidiTransmitCallback = callback;
    SYS_RestoreInterrupts();
    LOG("Gravis", "Midi transmit handler set");
}

void SetMidiReceiveCallback(const MidiReceiveCallback_t& callback)
{
    SYS_ClearInterrupts();
    s_MidiReceiveCallback = callback;
    SYS_RestoreInterrupts();
    LOG("Gravis", "Midi receive handler set");
}

bool SetTimer1Handler(const TimerCallback_t& callback, uint8_t ticksPerSecond)
{
    bool ret = false;
    if (SetupTimer1(ticksPerSecond))
    {
        SYS_ClearInterrupts();
        s_Timer1Callback = callback;
        SYS_RestoreInterrupts();
        StartTimer1();
        LOG("Gravis", "Timer 1 handler set");
        ret = true;
    }

    return ret;
}

bool SetTimer2Handler(const TimerCallback_t& callback, uint8_t ticksPerSecond)
{
    bool ret = false;
    if (SetupTimer2(ticksPerSecond))
    {
        SYS_ClearInterrupts();
        s_Timer2Callback = callback;
        SYS_RestoreInterrupts();
        StartTimer2();
        LOG("Gravis", "Timer 2 handler set");
        ret = true;
    }

    return ret;
}

bool s_Initialized = false;

InitializeError_t Initialize(Has::IAllocator& allocator)
{
    using namespace Has::System;

    if (s_Initialized)
        return InitializeError::AlreadyInitialized;

    s_Allocator = &allocator;

    Register_t baseAddress = 0;
    uint8_t playDma = 0;
    uint8_t recordDma = 0;
    uint8_t gf1Interrupt = 0;
    uint8_t midiInterrupt = 0;

    InitializeError_t ret = ParseEnvironmentVariable(baseAddress, playDma, recordDma, gf1Interrupt, midiInterrupt);

    if (ret == InitializeError::Success)
    {
        LOG("Gravis", "Environment read: base address: 0x%03X, play dma: %i, record dma: %i, gf1 irq: %i, midi irq: %i", baseAddress, playDma, recordDma, gf1Interrupt, midiInterrupt);

        if (DetectCard(baseAddress))
        {
            LOG("Gravis", "Card detected");

            ResetCard(baseAddress, 14);
            InitializeCard(baseAddress, playDma, recordDma, gf1Interrupt, midiInterrupt);
            InterruptTable::SetupHandler(s_UltrasoundInterrupt, UltrasoundInterruptHandler);
            if (s_UltrasoundInterrupt != s_MidiInterrupt)
                InterruptTable::SetupHandler(s_MidiInterrupt, MidiInterruptHandler);
            s_Initialized = true;
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
    using namespace Has::System;

    if (s_Initialized)
    {
        ShutdownTimers();
        MaskInterrupts(s_UltrasoundInterrupt, s_MidiInterrupt);
        ResetCard(s_BaseAddress, 14);
        GF1::Global::Reset::Write(s_BaseAddress, GF1::Global::Reset::MasterDisable | GF1::Global::Reset::InterruptDisable | GF1::Global::Reset::DacDisable);
        InterruptTable::RestoreHandler(s_UltrasoundInterrupt);
        if (s_UltrasoundInterrupt != s_MidiInterrupt)
            InterruptTable::RestoreHandler(s_MidiInterrupt);
        s_Initialized = false;
        LOG("Gravis", "Card shut down");
    }
}

}