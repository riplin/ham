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
    LOG("Gravis", "Select: %i", Index::Read(baseAddress));

    //Wait.
    Wait(baseAddress, 10);

    //Re-enable the card (silent).
    Reset::Write(baseAddress, Reset::MasterEnable | Reset::DacDisable | Reset::InterruptDisable);
    LOG("Gravis", "Select: %i", Index::Read(baseAddress));

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
    LOG("Gravis", "Select: %i", Index::Read(baseAddress));

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

void UploadSound(GF1::Global::DramIOAddress_t boardAddress, const void* data, uint32_t length)
{
    const uint8_t* ptr = static_cast<const uint8_t*>(data);
    //TODO: use dma. right now just poke it in.
    for (uint32_t i = 0; i < length; ++i)
    {
        WriteMemory(s_BaseAddress, boardAddress + i, *(ptr++));
    }
}

// When playing back 16 bit sound, the address granularity changes to 2 bytes per sample.
// The bank select bits (4 banks) stay in the same place though.
GF1::Global::DramIOAddress_t ConvertTo16BitAddress(GF1::Global::DramIOAddress_t address)
{
    GF1::Global::DramIOAddress_t bankSelect = address & GF1::Global::DramIOAddress::BankSelect;

    return ((address & ~GF1::Global::DramIOAddress::BankSelect) >> 1) | bankSelect;
}

GF1::Voice::VoiceControl_t SetVoice(GF1::Page_t voice,
                GF1::Global::DramIOAddress_t startLocation,
                GF1::Global::DramIOAddress_t loopStart,
                GF1::Global::DramIOAddress_t loopEnd,
                GF1::Voice::VoiceControl_t voiceControl, bool rollover)
{
    // If the loop points are reversed, we're playing backwards.
    if (loopStart > loopEnd)
    {
        GF1::Global::DramIOAddress_t t = loopStart;
        loopStart = loopEnd;
        loopEnd = t;
        voiceControl |= GF1::Voice::VoiceControl::Backward;
    }

    if ((voiceControl & GF1::Voice::VoiceControl::BitWidth) == GF1::Voice::VoiceControl::Bits16)
    {
        startLocation = ConvertTo16BitAddress(startLocation);
        loopStart = ConvertTo16BitAddress(loopStart);
        loopEnd = ConvertTo16BitAddress(loopEnd);
    }

    SYS_ClearInterrupts();
    GF1::Page::Write(s_BaseAddress, voice);

    if (rollover)
    {
        GF1::Voice::VolumeRampControl_t volumeRampControl = GF1::Voice::VolumeRampControl::Read(s_BaseAddress);
        volumeRampControl |= GF1::Voice::VolumeRampControl::RollOverCondition;
        GF1::Voice::VolumeRampControl::Write(s_BaseAddress, volumeRampControl);
        Wait(s_BaseAddress);
        GF1::Voice::VolumeRampControl::Write(s_BaseAddress, volumeRampControl);
    }

    GF1::Voice::CurrentLocation::Write(s_BaseAddress, startLocation << GF1::Voice::CurrentLocation::Shift::Address);
    GF1::Voice::StartLocation::Write(s_BaseAddress, loopStart << GF1::Voice::StartLocation::Shift::Address);
    GF1::Voice::EndLocation::Write(s_BaseAddress, loopEnd << GF1::Voice::EndLocation::Shift::Address);
    SYS_RestoreInterrupts();
    
    return voiceControl;
}

static GF1::Voice::CurrentVolume_t s_VolumeConversion[512] =
{
    0x0000,
    0x7000, 0x7ff0, 0x8800, 0x8ff0, 0x9400, 0x9800, 0x9c00, 0x9ff0, 0xa200,
    0xa400, 0xa600, 0xa800, 0xaa00, 0xac00, 0xae00, 0xaff0, 0xb100, 0xb200,
    0xb300, 0xb400, 0xb500, 0xb600, 0xb700, 0xb800, 0xb900, 0xba00, 0xbb00,
    0xbc00, 0xbd00, 0xbe00, 0xbf00, 0xbff0, 0xc080, 0xc100, 0xc180, 0xc200,
    0xc280, 0xc300, 0xc380, 0xc400, 0xc480, 0xc500, 0xc580, 0xc600, 0xc680,
    0xc700, 0xc780, 0xc800, 0xc880, 0xc900, 0xc980, 0xca00, 0xca80, 0xcb00,
    0xcb80, 0xcc00, 0xcc80, 0xcd00, 0xcd80, 0xce00, 0xce80, 0xcf00, 0xcf80,
    0xcff0, 0xd040, 0xd080, 0xd0c0, 0xd100, 0xd140, 0xd180, 0xd1c0, 0xd200,
    0xd240, 0xd280, 0xd2c0, 0xd300, 0xd340, 0xd380, 0xd3c0, 0xd400, 0xd440,
    0xd480, 0xd4c0, 0xd500, 0xd540, 0xd580, 0xd5c0, 0xd600, 0xd640, 0xd680,
    0xd6c0, 0xd700, 0xd740, 0xd780, 0xd7c0, 0xd800, 0xd840, 0xd880, 0xd8c0,
    0xd900, 0xd940, 0xd980, 0xd9c0, 0xda00, 0xda40, 0xda80, 0xdac0, 0xdb00,
    0xdb40, 0xdb80, 0xdbc0, 0xdc00, 0xdc40, 0xdc80, 0xdcc0, 0xdd00, 0xdd40,
    0xdd80, 0xddc0, 0xde00, 0xde40, 0xde80, 0xdec0, 0xdf00, 0xdf40, 0xdf80,
    0xdfc0, 0xdff0, 0xe020, 0xe040, 0xe060, 0xe080, 0xe0a0, 0xe0c0, 0xe0e0,
    0xe100, 0xe120, 0xe140, 0xe160, 0xe180, 0xe1a0, 0xe1c0, 0xe1e0, 0xe200,
    0xe220, 0xe240, 0xe260, 0xe280, 0xe2a0, 0xe2c0, 0xe2e0, 0xe300, 0xe320,
    0xe340, 0xe360, 0xe380, 0xe3a0, 0xe3c0, 0xe3e0, 0xe400, 0xe420, 0xe440,
    0xe460, 0xe480, 0xe4a0, 0xe4c0, 0xe4e0, 0xe500, 0xe520, 0xe540, 0xe560,
    0xe580, 0xe5a0, 0xe5c0, 0xe5e0, 0xe600, 0xe620, 0xe640, 0xe660, 0xe680,
    0xe6a0, 0xe6c0, 0xe6e0, 0xe700, 0xe720, 0xe740, 0xe760, 0xe780, 0xe7a0,
    0xe7c0, 0xe7e0, 0xe800, 0xe820, 0xe840, 0xe860, 0xe880, 0xe8a0, 0xe8c0,
    0xe8e0, 0xe900, 0xe920, 0xe940, 0xe960, 0xe980, 0xe9a0, 0xe9c0, 0xe9e0,
    0xea00, 0xea20, 0xea40, 0xea60, 0xea80, 0xeaa0, 0xeac0, 0xeae0, 0xeb00,
    0xeb20, 0xeb40, 0xeb60, 0xeb80, 0xeba0, 0xebc0, 0xebe0, 0xec00, 0xec20,
    0xec40, 0xec60, 0xec80, 0xeca0, 0xecc0, 0xece0, 0xed00, 0xed20, 0xed40,
    0xed60, 0xed80, 0xeda0, 0xedc0, 0xede0, 0xee00, 0xee20, 0xee40, 0xee60,
    0xee80, 0xeea0, 0xeec0, 0xeee0, 0xef00, 0xef20, 0xef40, 0xef60, 0xef80,
    0xefa0, 0xefc0, 0xefe0, 0xeff0, 0xf010, 0xf020, 0xf030, 0xf040, 0xf050,
    0xf060, 0xf070, 0xf080, 0xf090, 0xf0a0, 0xf0b0, 0xf0c0, 0xf0d0, 0xf0e0,
    0xf0f0, 0xf100, 0xf110, 0xf120, 0xf130, 0xf140, 0xf150, 0xf160, 0xf170,
    0xf180, 0xf190, 0xf1a0, 0xf1b0, 0xf1c0, 0xf1d0, 0xf1e0, 0xf1f0, 0xf200,
    0xf210, 0xf220, 0xf230, 0xf240, 0xf250, 0xf260, 0xf270, 0xf280, 0xf290,
    0xf2a0, 0xf2b0, 0xf2c0, 0xf2d0, 0xf2e0, 0xf2f0, 0xf300, 0xf310, 0xf320,
    0xf330, 0xf340, 0xf350, 0xf360, 0xf370, 0xf380, 0xf390, 0xf3a0, 0xf3b0,
    0xf3c0, 0xf3d0, 0xf3e0, 0xf3f0, 0xf400, 0xf410, 0xf420, 0xf430, 0xf440,
    0xf450, 0xf460, 0xf470, 0xf480, 0xf490, 0xf4a0, 0xf4b0, 0xf4c0, 0xf4d0,
    0xf4e0, 0xf4f0, 0xf500, 0xf510, 0xf520, 0xf530, 0xf540, 0xf550, 0xf560,
    0xf570, 0xf580, 0xf590, 0xf5a0, 0xf5b0, 0xf5c0, 0xf5d0, 0xf5e0, 0xf5f0,
    0xf600, 0xf610, 0xf620, 0xf630, 0xf640, 0xf650, 0xf660, 0xf670, 0xf680,
    0xf690, 0xf6a0, 0xf6b0, 0xf6c0, 0xf6d0, 0xf6e0, 0xf6f0, 0xf700, 0xf710,
    0xf720, 0xf730, 0xf740, 0xf750, 0xf760, 0xf770, 0xf780, 0xf790, 0xf7a0,
    0xf7b0, 0xf7c0, 0xf7d0, 0xf7e0, 0xf7f0, 0xf800, 0xf810, 0xf820, 0xf830,
    0xf840, 0xf850, 0xf860, 0xf870, 0xf880, 0xf890, 0xf8a0, 0xf8b0, 0xf8c0,
    0xf8d0, 0xf8e0, 0xf8f0, 0xf900, 0xf910, 0xf920, 0xf930, 0xf940, 0xf950,
    0xf960, 0xf970, 0xf980, 0xf990, 0xf9a0, 0xf9b0, 0xf9c0, 0xf9d0, 0xf9e0,
    0xf9f0, 0xfa00, 0xfa10, 0xfa20, 0xfa30, 0xfa40, 0xfa50, 0xfa60, 0xfa70,
    0xfa80, 0xfa90, 0xfaa0, 0xfab0, 0xfac0, 0xfad0, 0xfae0, 0xfaf0, 0xfb00,
    0xfb10, 0xfb20, 0xfb30, 0xfb40, 0xfb50, 0xfb60, 0xfb70, 0xfb80, 0xfb90,
    0xfba0, 0xfbb0, 0xfbc0, 0xfbd0, 0xfbe0, 0xfbf0, 0xfc00, 0xfc10, 0xfc20,
    0xfc30, 0xfc40, 0xfc50, 0xfc60, 0xfc70, 0xfc80, 0xfc90, 0xfca0, 0xfcb0,
    0xfcc0, 0xfcd0, 0xfce0, 0xfcf0, 0xfd00, 0xfd10, 0xfd20, 0xfd30, 0xfd40,
    0xfd50, 0xfd60, 0xfd70, 0xfd80, 0xfd90, 0xfda0, 0xfdb0, 0xfdc0, 0xfdd0,
    0xfde0, 0xfdf0, 0xfe00, 0xfe10, 0xfe20, 0xfe30, 0xfe40, 0xfe50, 0xfe60,
    0xfe70, 0xfe80, 0xfe90, 0xfea0, 0xfeb0, 0xfec0, 0xfed0, 0xfee0, 0xfef0,
    0xff00, 0xff10, 0xff20, 0xff30, 0xff40, 0xff50, 0xff60, 0xff70, 0xff80,
    0xff90, 0xffa0, 0xffb0, 0xffc0, 0xffd0, 0xffe0, 0xfff0
};

void SetVolume(GF1::Page_t voice, GF1::Voice::CurrentVolume_t volume)
{
    SYS_ClearInterrupts();
    GF1::Page::Write(s_BaseAddress, voice);
    GF1::Voice::CurrentVolume::Write(s_BaseAddress, volume);
    SYS_RestoreInterrupts();
}

void SetLinearVolume(GF1::Page_t voice, uint16_t volume)
{
    volume = Has::min<uint16_t>(volume, 511);
    volume = s_VolumeConversion[volume];
    SetVolume(voice,volume);
}

//Starts at 14.
static unsigned int s_FrequencyPerVoice[19] =
{
    44100,
    41160,
    38587,
    36317,
    34300,
    32494,
    30870,
    29400,
    28063,
    26843,
    25725,
    24696,
    23746,
    22866,
    22050,
    21289,
    20580,
    19916,
    19293
};

void SetPlaybackFrequency(GF1::Page_t voice, uint16_t frequencyInHz, uint16_t activeVoices)
{
    using namespace Has;
    uint8_t voiceCount = max<uint8_t>(min<uint8_t>(activeVoices, 32), 14);
    uint32_t divisor = s_FrequencyPerVoice[voiceCount - 14];
    uint32_t freq = ((uint32_t(frequencyInHz) << 9) + (divisor >> 1)) / divisor;
    freq <<= 1;
    
    SYS_ClearInterrupts();
    GF1::Page::Write(s_BaseAddress, voice);
    GF1::Voice::FrequencyControl::Write(s_BaseAddress, GF1::Voice::FrequencyControl_t(freq));
    SYS_RestoreInterrupts();
}

void SetPan(GF1::Page_t voice, GF1::Voice::PanPosition_t pan)
{
    using namespace Has;
    SYS_ClearInterrupts();
    GF1::Page::Write(s_BaseAddress, voice);
    GF1::Voice::PanPosition::Write(s_BaseAddress, min<GF1::Voice::PanPosition_t>(pan, 0xf));
    SYS_RestoreInterrupts();
}

void ResetVoice(GF1::Page_t voice)
{
    using namespace Has;
    using namespace GF1::Voice;
    SYS_ClearInterrupts();
    GF1::Page::Write(s_BaseAddress, voice);
    VoiceControl::Write(s_BaseAddress, GF1::Voice::VoiceControl::Stop | GF1::Voice::VoiceControl::Stopped);
    Wait(s_BaseAddress);
    VoiceControl::Write(s_BaseAddress, GF1::Voice::VoiceControl::Stop | GF1::Voice::VoiceControl::Stopped);
    FrequencyControl::Write(s_BaseAddress, 0x400);//1.0
    StartLocation::Write(s_BaseAddress, 0);
    EndLocation::Write(s_BaseAddress, 0);
    VolumeRampControl::Write(s_BaseAddress, VolumeRampControl::Stop | VolumeRampControl::Stopped);
    VolumeRampRate::Write(s_BaseAddress, 1);
    VolumeRamp::WriteStart(s_BaseAddress, 0x10);
    VolumeRamp::WriteEnd(s_BaseAddress, 0xe0);
    CurrentVolume::Write(s_BaseAddress, 0);
    CurrentLocation::Write(s_BaseAddress, 0);
    PanPosition::Write(s_BaseAddress, 7);
    SYS_RestoreInterrupts();
}

void PlayVoice(GF1::Page_t voice,
                GF1::Global::DramIOAddress_t startLocation,
                GF1::Global::DramIOAddress_t loopStart,
                GF1::Global::DramIOAddress_t loopEnd,
                GF1::Voice::VoiceControl_t voiceControl, bool rollover)
{
    voiceControl = SetVoice(voice, startLocation, loopStart, loopEnd, voiceControl, rollover);
    voiceControl &= ~(GF1::Voice::VoiceControl::Stop | GF1::Voice::VoiceControl::Stopped);

    SYS_ClearInterrupts();
    GF1::Page::Write(s_BaseAddress, voice);
    GF1::Voice::VoiceControl::Write(s_BaseAddress, voiceControl);
    Wait(s_BaseAddress);
    GF1::Voice::VoiceControl::Write(s_BaseAddress, voiceControl);
    SYS_RestoreInterrupts();
}

void StopVoice(GF1::Page_t voice)
{
    SYS_ClearInterrupts();
    GF1::Page::Write(s_BaseAddress, voice);
    GF1::Voice::VoiceControl::Write(s_BaseAddress, GF1::Voice::VoiceControl::Stop | GF1::Voice::VoiceControl::Stopped);
    Wait(s_BaseAddress);
    GF1::Voice::VoiceControl::Write(s_BaseAddress, GF1::Voice::VoiceControl::Stop | GF1::Voice::VoiceControl::Stopped);
    SYS_RestoreInterrupts();
}

void ResumeVoice(GF1::Page_t voice)
{
    SYS_ClearInterrupts();
    GF1::Page::Write(s_BaseAddress, voice);
    GF1::Voice::VoiceControl_t voiceControl = GF1::Voice::VoiceControl::Read(s_BaseAddress) & ~(GF1::Voice::VoiceControl::Stop | GF1::Voice::VoiceControl::Stopped);
    GF1::Voice::VoiceControl::Write(s_BaseAddress, voiceControl);
    Wait(s_BaseAddress);
    GF1::Voice::VoiceControl::Write(s_BaseAddress, voiceControl);
    SYS_RestoreInterrupts();
}

void InitializeCard(Register_t baseAddress, uint8_t playDma, uint8_t recordDma, uint8_t gf1Interrupt, uint8_t midiInterrupt)
{
    s_BaseAddress = baseAddress;
    s_PlayDMA = playDma;
    s_RecordDMA = recordDma;
    s_UltrasoundInterrupt = gf1Interrupt;
    s_MidiInterrupt = midiInterrupt;

    GF1::MixControl_t mixControl = GF1::MixControl::LineInputDisable | GF1::MixControl::LineOutputDisable |
                                   GF1::MixControl::MicrophoneInputDisable | GF1::MixControl::LatchesEnable;

    GF1::InterruptControl_t interruptControl = s_InterruptLatchValues[gf1Interrupt & 0xf];
    GF1::InterruptControl_t midiInterruptLatch = s_InterruptLatchValues[midiInterrupt & 0xf] << GF1::InterruptControl::Shift::MidiInterruptSelector;

    GF1::DmaControl_t dmaControl = s_DmaLatchValues[playDma];
    GF1::DmaControl_t recordDmaLatch = s_DmaLatchValues[recordDma] << GF1::DmaControl::Shift::Dma2Selector;

    if ((gf1Interrupt == midiInterrupt) && (gf1Interrupt != 0))
    {
        interruptControl |= GF1::InterruptControl::CombineInterrupts;
        LOG("Gravis", "Interrupts for GF1 and Midi are shared");
    }
    else
    {
        interruptControl |= midiInterruptLatch;
    }

    if ((playDma == recordDma) && (playDma != 0))
    {
        dmaControl |= GF1::DmaControl::CombineDma;
        LOG("Gravis", "Dma channels for GF1 and Midi are shared");
    }
    else
    {
        dmaControl |= recordDmaLatch;
    }

    SYS_ClearInterrupts();
    GF1::MixControl::Write(s_BaseAddress, mixControl);

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

    //Enable DAC, enable interrupts.
    GF1::Global::Reset::Write(baseAddress, GF1::Global::Reset::MasterEnable | GF1::Global::Reset::DacEnable | GF1::Global::Reset::InterruptEnable);

    SYS_RestoreInterrupts();
}

void UnmaskInterrupts(uint8_t gf1Interrupt, uint8_t midiInterrupt)
{
    using namespace Has::System;

    //Unmask the system interrupts
    if (gf1Interrupt != 0)
        PIC::Mask::UnmaskInterrupt(gf1Interrupt);

    if ((midiInterrupt != gf1Interrupt) && (midiInterrupt != 0))
        PIC::Mask::UnmaskInterrupt(midiInterrupt);
    
	//Unmask cascade interrupt 2
    if (gf1Interrupt > 7 || midiInterrupt > 7)
        PIC::Mask::UnmaskInterrupt(2);
}

void MaskInterrupts(uint8_t gf1Interrupt, uint8_t midiInterrupt)
{
    using namespace Has::System;

    if ((gf1Interrupt != 0) && (gf1Interrupt != 2))
        PIC::Mask::MaskInterrupt(gf1Interrupt);

    if ((midiInterrupt != 0) && (midiInterrupt != 2))
        PIC::Mask::MaskInterrupt(midiInterrupt);

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

void StopTimers()
{
    SYS_ClearInterrupts();
    uint8_t tries = 16;
    do
    {
        GF1::TimerControl::Write(s_BaseAddress, GF1::TimerControl::TimerControlSelect);
        GF1::TimerData::Write(s_BaseAddress, 0);
        GF1::TimerData::Write(s_BaseAddress, GF1::TimerData::Timer1Mask | GF1::TimerData::Timer2Mask);
        GF1::TimerData::Write(s_BaseAddress, GF1::TimerData::ClearTimerInterrupt | GF1::TimerData::Timer1Mask | GF1::TimerData::Timer2Mask);
        GF1::TimerData::Write(s_BaseAddress, GF1::TimerData::ClearTimerInterrupt);
    } while (--tries != 0 && ((GF1::TimerControl::Read(s_BaseAddress) & 
            (GF1::TimerControl::Timer1Expired | GF1::TimerControl::Timer2Expired | GF1::TimerControl::TimersExpired)) != 0));

    GF1::Global::TimerControl::Write(s_BaseAddress, GF1::Global::TimerControl::Timer1Enable | GF1::Global::TimerControl::Timer2Enable);
    GF1::Global::TimerCount::Write1(s_BaseAddress, 0);
    GF1::Global::TimerCount::Write2(s_BaseAddress, 0);
    GF1::Global::TimerControl::Write(s_BaseAddress, GF1::Global::TimerControl::Timer1Disable | GF1::Global::TimerControl::Timer2Disable);
    SYS_RestoreInterrupts();

    LOG("Gravis", "Timer stop tries needed: %i", 16 - tries);
}

bool StartTimers(uint8_t ticksPerSecond1, uint8_t ticksPerSecond2)
{
    uint16_t interval1 = 12500 / ticksPerSecond1;
    if (interval1 > 256)
    {
        LOG("Gravis", "Error setting up timer 1, can't count up from %i, ticks per second: %i", interval1, ticksPerSecond1);
        return false;
    }

    uint16_t interval2 = 3125 / ticksPerSecond2;
    if (interval2 > 256)
    {
        LOG("Gravis", "Error setting up timer 2, can't count up from %i, ticks per second: %i", interval2, ticksPerSecond2);
        return false;
    }

    int16_t countStart1 = 256 - interval1;
    int16_t countStart2 = 256 - interval2;

    StopTimers();

    SYS_ClearInterrupts();
    GF1::Global::TimerControl::Write(s_BaseAddress, GF1::Global::TimerControl::Timer1Enable | GF1::Global::TimerControl::Timer2Enable);
    GF1::Global::TimerCount::Write1(s_BaseAddress, GF1::Global::TimerCount_t(countStart1));
    GF1::Global::TimerCount::Write2(s_BaseAddress, GF1::Global::TimerCount_t(countStart2));
    GF1::TimerControl::Write(s_BaseAddress, GF1::TimerControl::TimerControlSelect);
    GF1::TimerData::Write(s_BaseAddress, GF1::TimerData::ClearTimerInterrupt | GF1::TimerData::Timer1Mask | GF1::TimerData::Timer2Mask);
    GF1::TimerData::Write(s_BaseAddress, GF1::TimerData::Timer1Mask | GF1::TimerData::Timer2Mask);
    GF1::TimerData::Write(s_BaseAddress, 0);
    GF1::TimerData::Write(s_BaseAddress, GF1::TimerData::Timer1Start | GF1::TimerData::Timer2Start);
    SYS_RestoreInterrupts();

    LOG("Gravis", "Timer 1 set up: %i ticks per second, count up from: %i", ticksPerSecond1, countStart1);
    LOG("Gravis", "Timer 2 set up: %i ticks per second, count up from: %i", ticksPerSecond2, countStart2);
    return true;
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

void SetTimer1Handler(const TimerCallback_t& callback, uint8_t ticksPerSecond)
{
    SYS_ClearInterrupts();
    s_Timer1Callback = callback;
    SYS_RestoreInterrupts();
    LOG("Gravis", "Timer 1 handler set");
}

void SetTimer2Handler(const TimerCallback_t& callback, uint8_t ticksPerSecond)
{
    SYS_ClearInterrupts();
    s_Timer2Callback = callback;
    SYS_RestoreInterrupts();
}

bool s_Initialized = false;

InitializeError_t Initialize(Has::IAllocator& allocator)
{
    using namespace Has;
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
            // InterruptTable::SetupHandler(s_UltrasoundInterrupt, UltrasoundInterruptHandler);
            // if (s_UltrasoundInterrupt != s_MidiInterrupt)
            //     InterruptTable::SetupHandler(s_MidiInterrupt, MidiInterruptHandler);
            // UnmaskInterrupts(gf1Interrupt, midiInterrupt);
            StartTimers(50, 100);
            s_Initialized = true;
        }
        else
        {
            ret = InitializeError::CardNotFound;
        }
        LOG("Gravis", "Card initialized");
    }

    return ret;
}

void Configure(uint8_t activeVoices)
{
    if (s_Initialized)
    {
        ResetCard(s_BaseAddress, activeVoices);
    }
}

void Shutdown()
{
    using namespace Has::System;

    if (s_Initialized)
    {
        LOG("Gravis", "Card shutting down");
        StopTimers();
        // MaskInterrupts(s_UltrasoundInterrupt, s_MidiInterrupt);
        // InterruptTable::RestoreHandler(s_UltrasoundInterrupt);
        // if (s_UltrasoundInterrupt != s_MidiInterrupt)
        //     InterruptTable::RestoreHandler(s_MidiInterrupt);
        ResetCard(s_BaseAddress, 14);
        GF1::Global::Reset::Write(s_BaseAddress, GF1::Global::Reset::MasterDisable | GF1::Global::Reset::InterruptDisable | GF1::Global::Reset::DacDisable);
        s_Initialized = false;
        LOG("Gravis", "Card shut down");
    }
}

}