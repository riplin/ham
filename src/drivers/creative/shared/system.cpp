//Copyright 2025-Present riplin

#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <support/env.h>
#include <ham/drivers/creative/shared/system.h>
#include <ham/drivers/creative/shared/emu8k/base.h>
#include <ham/drivers/creative/shared/emu8k/data.h>
#include <ham/drivers/creative/shared/emu8k/voice/ip.h>
#include <ham/drivers/creative/shared/emu8k/voice/csl.h>
#include <ham/drivers/creative/shared/emu8k/voice/cpf.h>
#include <ham/drivers/creative/shared/emu8k/global/wc.h>
#include <ham/drivers/creative/shared/emu8k/global/smd.h>
#include <ham/drivers/creative/shared/emu8k/voice/cvcf.h>
#include <ham/drivers/creative/shared/emu8k/voice/init.h>
#include <ham/drivers/creative/shared/emu8k/voice/pefe.h>
#include <ham/drivers/creative/shared/emu8k/voice/ptrx.h>
#include <ham/drivers/creative/shared/emu8k/voice/vtft.h>
#include <ham/drivers/creative/shared/emu8k/voice/psst.h>
#include <ham/drivers/creative/shared/emu8k/voice/ccca.h>
#include <ham/drivers/creative/shared/emu8k/global/smar.h>
#include <ham/drivers/creative/shared/emu8k/global/smaw.h>
#include <ham/drivers/creative/shared/emu8k/voice/ifatn.h>
#include <ham/drivers/creative/shared/emu8k/voice/fmmod.h>
#include <ham/drivers/creative/shared/emu8k/global/hwcfg.h>
#include <ham/drivers/creative/shared/emu8k/voice/envvol.h>
#include <ham/drivers/creative/shared/emu8k/voice/envval.h>
#include <ham/drivers/creative/shared/emu8k/voice/dcysus.h>
#include <ham/drivers/creative/shared/emu8k/voice/lfoval.h>
#include <ham/drivers/creative/shared/emu8k/voice/atkhld.h>
#include <ham/drivers/creative/shared/emu8k/voice/dcysusv.h>
#include <ham/drivers/creative/shared/emu8k/voice/atkhldv.h>
#include <ham/drivers/creative/shared/emu8k/voice/tremfrq.h>
#include <ham/drivers/creative/shared/emu8k/voice/fm2frq2.h>
namespace Ham::Creative::Shared::Functions::System
{

bool DetectCard(Register_t baseAddress)
{
    Emu8k::Data::Read3Word(baseAddress, 7, 0); //Discard
    if ((Emu8k::Data::Read3Word(baseAddress, 7, 0) & 0x0F) != 0x0C)
        return false;

    if ((Emu8k::Global::Configuration::Read1(baseAddress) & 0x7e) != 0x58)
        return false;

    if ((Emu8k::Global::Configuration::Read2(baseAddress) & 0x03) != 0x03)
        return false;

    return false;
}

static void split(const std::string &s, char delim, std::vector<std::string>& result)
{
    std::istringstream iss(s);
    std::string item;

    while (std::getline(iss, item, delim))
        if (!item.empty()) result.push_back(item);
}

static std::string tolower(const std::string& s)
{
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), [](uint8_t c){ return std::tolower(c); });
    return r;
}

InitializeError_t ParseEnvironmentVariable(Register_t& baseAddress, uint8_t& lowDma, uint8_t& highDma, uint8_t& interrupt, Register_t& mixerBase, Register_t& midiBase, CardType_t& type, Register_t& emuBase)
{
    baseAddress = 0;
    lowDma = 0xFF;
    highDma = 0;
    interrupt = 0;
    mixerBase = 0;
    midiBase = 0;
    type = CardType::Unknown;
    emuBase = 0;
    //A220 I5 D1 H5 M220 P330 E640 T6
    const char* blaster = Support::GetEnv("BLASTER");
    if (blaster != nullptr)
    {
        std::vector<std::string> values;
        split(tolower(blaster), ' ', values);
        for (const auto& value : values)
        {
            if ((value.length() == 4) &&
                (value[0] == 'a') &&
                (value[1] == '2') &&
                (value[2] >= '2') &&
                (value[2] <= '8') &&
                (value[3] == '0'))
            {
                baseAddress = (Register_t(value[1] - '0') << 8) | (Register_t(value[2] - '0') << 4) | Register_t(value[3] - '0');
            }

            if ((value.length() == 2) &&
                (value[0] == 'i') &&
                (value[1] >= '0') &&
                (value[1] <= '9'))
            {
                interrupt = value[1] - '0';
            }

            if ((value.length() == 2) &&
                (value[0] == 'd') &&
                (value[1] >= '0') &&
                (value[1] <= '3'))
            {
                lowDma = value[1] - '0';
            }

            if ((value.length() == 2) &&
                (value[0] == 'h') &&
                (value[1] >= '5') &&
                (value[1] <= '7'))
            {
                highDma = value[1] - '0';
            }

            if ((value.length() == 4) &&
                (value[0] == 'm') &&
                (value[1] == '2') &&
                (value[2] >= '2') &&
                (value[2] <= '8') &&
                (value[3] == '0'))
            {
                mixerBase = (Register_t(value[1] - '0') << 8) | (Register_t(value[2] - '0') << 4) | Register_t(value[3] - '0');
            }

            if ((value.length() == 4) &&
                (value[0] == 'p') &&
                (value[1] == '3') &&
                (value[2] >= '0') &&
                (value[2] <= '3') &&
                (value[3] == '0'))
            {
                midiBase = (Register_t(value[1] - '0') << 8) | (Register_t(value[2] - '0') << 4) | Register_t(value[3] - '0');
            }

            if ((value.length() == 4) &&
                (value[0] == 'e') &&
                (value[1] == '6') &&
                (value[2] >= '2') &&
                (value[2] <= '8') &&
                (value[3] == '0'))
            {
                emuBase = (Register_t(value[1] - '0') << 8) | (Register_t(value[2] - '0') << 4) | Register_t(value[3] - '0');
            }

            if ((value.length() == 2) &&
                (value[0] == 't') &&
                (value[1] >= '1') &&
                (value[1] <= '6'))
            {
                type = value[1] - '0';
            }
        }

        if (baseAddress == 0)
            return InitializeError::BlasterEnvMalformed;

        if ((baseAddress != 0x220) &&
            (baseAddress != 0x240) &&
            (baseAddress != 0x260) &&
            (baseAddress != 0x280))
            return InitializeError::BlasterEnvInvalidValues;

        if (mixerBase == 0)
            mixerBase = baseAddress;

        if ((mixerBase != 0x220) &&
            (mixerBase != 0x240) &&
            (mixerBase != 0x260) &&
            (mixerBase != 0x280))
            return InitializeError::BlasterEnvInvalidValues;

        if (lowDma == 0xFF)
            return InitializeError::BlasterEnvMalformed;

        if ((lowDma != 0) &&
            (lowDma != 1) &&
            (lowDma != 3))
            return InitializeError::BlasterEnvInvalidValues;

        if ((highDma != 5) &&
            (highDma != 6) &&
            (highDma != 7))
            return InitializeError::BlasterEnvInvalidValues;

        if (interrupt == 0)
            return InitializeError::BlasterEnvMalformed;

        if ((interrupt != 2) &&
            (interrupt != 5) &&
            (interrupt != 7) &&
            (interrupt != 10))
            return InitializeError::BlasterEnvInvalidValues;

        if (midiBase == 0)
            return InitializeError::BlasterEnvMalformed;

        if ((midiBase != 0x300) &&
            (midiBase != 0x330))
            return InitializeError::BlasterEnvInvalidValues;

        if (type == CardType::Unknown)
            type = CardType::SoundBlaster;

        if (emuBase == 0)
            emuBase = baseAddress + Emu8k::Register::Base;

        if ((emuBase != 0x620) &&
            (emuBase != 0x640) &&
            (emuBase != 0x660) &&
            (emuBase != 0x680))
            return InitializeError::BlasterEnvInvalidValues;

        return InitializeError::Success;
    }

    return InitializeError::BlasterEnvNotFound;
}

bool s_Initialized = false;
Has::IAllocator* s_Allocator = nullptr;
Register_t s_BaseAddress = 0;
uint8_t s_LowDma = 0;
uint8_t s_HighDma = 0;
uint8_t s_Interrupt = 0;
Register_t s_MixerBase = 0;
Register_t s_MidiBase = 0;
CardType_t s_Type = 0;
Register_t s_EmuBase = 0;
uint32_t s_MemoryAvailable = 0;

uint32_t GetAvailableMemory()
{
    return s_MemoryAvailable;
}

// The initialization arrays to be written to INIT1 through INIT4 contain parameters determining
// the function of the reverb, chorus and equalization effects performed by the EMU8000. The
// following values supply the standard default AWE32 “Hall 2” reverb and “Chorus 3” chorus
// programs, and with a flat equalization curve:

// The initialization arrays are as follows:

// First initialization set:
static Emu8k::Voice::Initialization_t s_InitializationStep1Set1[32] =
{
    0x03ff, 0x0030, 0x07ff, 0x0130, 0x0bff, 0x0230, 0x0fff, 0x0330,
    0x13ff, 0x0430, 0x17ff, 0x0530, 0x1bff, 0x0630, 0x1fff, 0x0730,
    0x23ff, 0x0830, 0x27ff, 0x0930, 0x2bff, 0x0a30, 0x2fff, 0x0b30,
    0x33ff, 0x0c30, 0x37ff, 0x0d30, 0x3bff, 0x0e30, 0x3fff, 0x0f30
};

static Emu8k::Voice::Initialization_t s_InitializationStep1Set2[32] =
{
    0x43ff, 0x0030, 0x47ff, 0x0130, 0x4bff, 0x0230, 0x4fff, 0x0330,
    0x53ff, 0x0430, 0x57ff, 0x0530, 0x5bff, 0x0630, 0x5fff, 0x0730,
    0x63ff, 0x0830, 0x67ff, 0x0930, 0x6bff, 0x0a30, 0x6fff, 0x0b30,
    0x73ff, 0x0c30, 0x77ff, 0x0d30, 0x7bff, 0x0e30, 0x7fff, 0x0f30
};

static Emu8k::Voice::Initialization_t s_InitializationStep1Set3[32] =
{
    0x83ff, 0x0030, 0x87ff, 0x0130, 0x8bff, 0x0230, 0x8fff, 0x0330,
    0x93ff, 0x0430, 0x97ff, 0x0530, 0x9bff, 0x0630, 0x9fff, 0x0730,
    0xa3ff, 0x0830, 0xa7ff, 0x0930, 0xabff, 0x0a30, 0xafff, 0x0b30,
    0xb3ff, 0x0c30, 0xb7ff, 0x0d30, 0xbbff, 0x0e30, 0xbfff, 0x0f30
};

static Emu8k::Voice::Initialization_t s_InitializationStep1Set4[32] =
{
    0xc3ff, 0x0030, 0xc7ff, 0x0130, 0xcbff, 0x0230, 0xcfff, 0x0330,
    0xd3ff, 0x0430, 0xd7ff, 0x0530, 0xdbff, 0x0630, 0xdfff, 0x0730,
    0xe3ff, 0x0830, 0xe7ff, 0x0930, 0xebff, 0x0a30, 0xefff, 0x0b30,
    0xf3ff, 0x0c30, 0xf7ff, 0x0d30, 0xfbff, 0x0e30, 0xffff, 0x0f30
};

static Emu8k::Voice::Initialization_t s_InitializationStep2Set1[32] =
{
    0x03ff, 0x8030, 0x07ff, 0x8130, 0x0bff, 0x8230, 0x0fff, 0x8330,
    0x13ff, 0x8430, 0x17ff, 0x8530, 0x1bff, 0x8630, 0x1fff, 0x8730,
    0x23ff, 0x8830, 0x27ff, 0x8930, 0x2bff, 0x8a30, 0x2fff, 0x8b30,
    0x33ff, 0x8c30, 0x37ff, 0x8d30, 0x3bff, 0x8e30, 0x3fff, 0x8f30
};

static Emu8k::Voice::Initialization_t s_InitializationStep2Set2[32] =
{
    0x43ff, 0x8030, 0x47ff, 0x8130, 0x4bff, 0x8230, 0x4fff, 0x8330,
    0x53ff, 0x8430, 0x57ff, 0x8530, 0x5bff, 0x8630, 0x5fff, 0x8730,
    0x63ff, 0x8830, 0x67ff, 0x8930, 0x6bff, 0x8a30, 0x6fff, 0x8b30,
    0x73ff, 0x8c30, 0x77ff, 0x8d30, 0x7bff, 0x8e30, 0x7fff, 0x8f30
};

static Emu8k::Voice::Initialization_t s_InitializationStep2Set3[32] =
{
    0x83ff, 0x8030, 0x87ff, 0x8130, 0x8bff, 0x8230, 0x8fff, 0x8330,
    0x93ff, 0x8430, 0x97ff, 0x8530, 0x9bff, 0x8630, 0x9fff, 0x8730,
    0xa3ff, 0x8830, 0xa7ff, 0x8930, 0xabff, 0x8a30, 0xafff, 0x8b30,
    0xb3ff, 0x8c30, 0xb7ff, 0x8d30, 0xbbff, 0x8e30, 0xbfff, 0x8f30
};

static Emu8k::Voice::Initialization_t s_InitializationStep2Set4[32] =
{
    0xc3ff, 0x8030, 0xc7ff, 0x8130, 0xcbff, 0x8230, 0xcfff, 0x8330,
    0xd3ff, 0x8430, 0xd7ff, 0x8530, 0xdbff, 0x8630, 0xdfff, 0x8730,
    0xe3ff, 0x8830, 0xe7ff, 0x8930, 0xebff, 0x8a30, 0xefff, 0x8b30,
    0xf3ff, 0x8c30, 0xf7ff, 0x8d30, 0xfbff, 0x8e30, 0xffff, 0x8f30
};

static Emu8k::Voice::Initialization_t s_InitializationStep3Set1[32] =
{
    0x0C10, 0x8470, 0x14FE, 0xB488, 0x167F, 0xA470, 0x18E7, 0x84B5,
    0x1B6E, 0x842A, 0x1F1D, 0x852A, 0x0DA3, 0x9F7C, 0x167E, 0xF254,
    0x0000, 0x842A, 0x0001, 0x852A, 0x18E6, 0x9BAA, 0x1B6D, 0xF234,
    0x229F, 0x8429, 0x2746, 0x8529, 0x1F1C, 0x96E7, 0x229E, 0xF224
};

static Emu8k::Voice::Initialization_t s_InitializationStep3Set2[32] =
{
    0x0DA4, 0x8429, 0x2C29, 0x8529, 0x2745, 0x97F6, 0x2C28, 0xF254,
    0x383B, 0x8428, 0x320F, 0x8528, 0x320E, 0x9F02, 0x1341, 0xF264,
    0x3EB6, 0x8428, 0x3EB9, 0x8528, 0x383A, 0x9FA9, 0x3EB5, 0xF294,
    0x3EB7, 0x8474, 0x3EBA, 0x8575, 0x3EB8, 0xC4C3, 0x3EBB, 0xC5C3
};

static Emu8k::Voice::Initialization_t s_InitializationStep3Set3[32] =
{
    0x0000, 0xA404, 0x0001, 0xA504, 0x141F, 0x8671, 0x14FD, 0x8287,
    0x3EBC, 0xE610, 0x3EC8, 0x8C7B, 0x031A, 0x87E6, 0x3EC8, 0x86F7,
    0x3EC0, 0x821E, 0x3EBE, 0xD208, 0x3EBD, 0x821F, 0x3ECA, 0x8386,
    0x3EC1, 0x8C03, 0x3EC9, 0x831E, 0x3ECA, 0x8C4C, 0x3EBF, 0x8C55
};

static Emu8k::Voice::Initialization_t s_InitializationStep3Set4[32] =
{
    0x3EC9, 0xC208, 0x3EC4, 0xBC84, 0x3EC8, 0x8EAD, 0x3EC8, 0xD308,
    0x3EC2, 0x8F7E, 0x3ECB, 0x821E, 0x3ECB, 0xD208, 0x3EC5, 0x831F,
    0x3EC6, 0xC308, 0x3EC3, 0xB2FF, 0x3EC9, 0x8265, 0x3EC9, 0x831E,
    0x1342, 0xD308, 0x3EC7, 0xB3FF, 0x0000, 0x8365, 0x1420, 0x9570
};

static Emu8k::Voice::Initialization_t s_InitializationStep4Set1[32] =
{
    0x0C10, 0x8470, 0x14FE, 0xB488, 0x167F, 0xA470, 0x18E7, 0x84B5,
    0x1B6E, 0x842A, 0x1F1D, 0x852A, 0x0DA3, 0x0F7C, 0x167E, 0x7254,
    0x0000, 0x842A, 0x0001, 0x852A, 0x18E6, 0x0BAA, 0x1B6D, 0x7234,
    0x229F, 0x8429, 0x2746, 0x8529, 0x1F1C, 0x06E7, 0x229E, 0x7224
};

static Emu8k::Voice::Initialization_t s_InitializationStep4Set2[32] =
{
    0x0DA4, 0x8429, 0x2C29, 0x8529, 0x2745, 0x07F6, 0x2C28, 0x7254,
    0x383B, 0x8428, 0x320F, 0x8528, 0x320E, 0x0F02, 0x1341, 0x7264,
    0x3EB6, 0x8428, 0x3EB9, 0x8528, 0x383A, 0x0FA9, 0x3EB5, 0x7294,
    0x3EB7, 0x8474, 0x3EBA, 0x8575, 0x3EB8, 0x44C3, 0x3EBB, 0x45C3
};

static Emu8k::Voice::Initialization_t s_InitializationStep4Set3[32] =
{
    0x0000, 0xA404, 0x0001, 0xA504, 0x141F, 0x0671, 0x14FD, 0x0287,
    0x3EBC, 0xE610, 0x3EC8, 0x0C7B, 0x031A, 0x07E6, 0x3EC8, 0x86F7,
    0x3EC0, 0x821E, 0x3EBE, 0xD208, 0x3EBD, 0x021F, 0x3ECA, 0x0386,
    0x3EC1, 0x0C03, 0x3EC9, 0x031E, 0x3ECA, 0x8C4C, 0x3EBF, 0x0C55
};

static Emu8k::Voice::Initialization_t s_InitializationStep4Set4[32] =
{
    0x3EC9, 0xC208, 0x3EC4, 0xBC84, 0x3EC8, 0x0EAD, 0x3EC8, 0xD308,
    0x3EC2, 0x8F7E, 0x3ECB, 0x021E, 0x3ECB, 0xD208, 0x3EC5, 0x031F,
    0x3EC6, 0xC308, 0x3EC3, 0x32FF, 0x3EC9, 0x0265, 0x3EC9, 0x831E,
    0x1342, 0xD308, 0x3EC7, 0x33FF, 0x0000, 0x8365, 0x1420, 0x9570
};

void Wait(unsigned int waitTime)
{
    using namespace Emu8k::Global;

	SampleCounter_t startTime = SampleCounter::Read(s_EmuBase);
	SampleCounter_t currentTime = startTime;
	while(waitTime > SampleCounter_t(currentTime - startTime))
    {
		currentTime = SampleCounter::Read(s_EmuBase);
    }
}

void InitializeCard(Register_t baseAddress, uint8_t lowDma, uint8_t highDma, uint8_t interrupt, Register_t mixerBase, Register_t midiBase, CardType_t type, Register_t emuBase)
{
    using namespace Emu8k::Voice;
    using namespace Emu8k::Global;
    s_BaseAddress = baseAddress;
    s_LowDma = lowDma;
    s_HighDma = highDma;
    s_Interrupt = interrupt;
    s_MixerBase = mixerBase;
    s_MidiBase = midiBase;
    s_Type = type;
    s_EmuBase = emuBase;

    // On power-up, most EMU8000 registers contain random data. Register HWCF3 contains a
    // bit which enables audio output which is cleared on reset. The proper procedure for initializing
    // the EMU8000 is to:
    //     1) Initialize the hardware configuration by writing registers HWCF1 and HWCF2.
    //     2) Initialize the audio channels by writing default values to all channel registers.
    //     3) Initialize the initialization arrays and the sound memory DMA address registers.
    //     4) Enable audio by writing register HWCF3.



    // On power-up, register HWCF1 should be written with value 0x0059 and register HWCF2 with
    // value 0x0020. Note that due to a VLSI error, these registers will not be correctly read by the
    // processor, so the actual values programmed cannot be easily verified.

    Configuration::Write3(s_EmuBase, 0x0000);    // Disable output
    Configuration::Write1(s_EmuBase, 0x0059);
    Configuration::Write2(s_EmuBase, 0x0020);

    // The channels should next be initialized. The order in which registers are initialized does matter,
    // since the envelope engine registers can alter the sound engine registers, and the sound engine
    // registers can alter each other.

    // Step 1: For all channels, set DCYSUSV to 0x0080. This turns off the envelope engine.
    for (uint8_t channel = 0; channel < 32; ++channel)
        VolumeEnvelopeSustainAndDecay::Write(s_EmuBase, channel, VolumeEnvelopeSustainAndDecay::EnvelopeGeneratorDisable);

    // Step 2: For all channels, set the following registers to zero: ENVVOL, ENVVAL, DCYSUS,
    //         ATKHLDV, LFO1VAL, ATKHLD, LFO2VAL, IP, IFATN, PEFE, FMMOD,
    //         TREMFRQ, FM2FRQ2, PTRX, VTFT, PSST, CSL, CCCA. This intializes all envelope
    //         and sound engine registers except “current” registers, which must be intialized last.
    for (uint8_t channel = 0; channel < 32; ++channel)
    {
        VolumeEnvelopeHoldAndAttack::Write(s_EmuBase, channel, VolumeEnvelopeHoldAndAttack::Attack6msec | VolumeEnvelopeHoldAndAttack::HoldNever);
        LfoDelay::Write1(s_EmuBase, channel, LfoDelay::NoDelay);
        ModulationEnvelopeHoldAndAttack::Write(s_EmuBase, channel, ModulationEnvelopeHoldAndAttack::Attack6msec | ModulationEnvelopeHoldAndAttack::HoldNever);
        LfoDelay::Write2(s_EmuBase, channel, LfoDelay::NoDelay);
        InitialPitch::Write(s_EmuBase, channel, InitialPitch::NoPitchShift);
        InitialFilterCutoffAndAttenuation::Write(s_EmuBase, channel, 0);
        PitchAndFilterEnvelopeHeights::Write(s_EmuBase, channel, 0);
        VibratoAndFilterModulationFromLfo1::Write(s_EmuBase, channel, 0);
        Lfo1TremoloAmountAndFrequency::Write(s_EmuBase, channel, 0);
        Lfo2VibratoAmountAndFrequency::Write(s_EmuBase, channel, 0);
        PitchTargetReverbSendAndAux::Write(s_EmuBase, channel, 0);
        VolumeAndFilterCutoffTargets::Write(s_EmuBase, channel, 0);
        PanSendAndLoopStartAddress::Write(s_EmuBase, channel, 0);
        ChorusSendAndLoopEndAddress::Write(s_EmuBase, channel, 0);
        QControlBitsAndCurrentAddress::Write(s_EmuBase, channel, 0);
    }

    // Step 3: For all channels, set the following registers to zero: CPF, CVCF. This initializes the
    //         “current” registers.
    for (uint8_t channel = 0; channel < 32; ++channel)
    {
        CurrentPitchAndFractionalAddress::Write(s_EmuBase, channel, 0);
        CurrentVolumeAndFilterCutoff::Write(s_EmuBase, channel, 0);
    }

    // The channels are now initialized, and the sound memory DMA registers can be initialized by
    // writing zero to SMALR, SMARR, SMALW, SMARW. 
    SoundMemoryAddressRead::WriteLeft(s_EmuBase, 0);
    SoundMemoryAddressRead::WriteRight(s_EmuBase, 0);
    SoundMemoryAddressWrite::WriteLeft(s_EmuBase, 0);
    SoundMemoryAddressWrite::WriteRight(s_EmuBase, 0);

    // Next the initialization arrays are set, by the following sequence:    

    // Step 1: Copy the first set of initialization arrays to INIT1 through INIT4.
    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write1(s_EmuBase, channel, s_InitializationStep1Set1[channel]);

    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write2(s_EmuBase, channel, s_InitializationStep1Set2[channel]);

    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write3(s_EmuBase, channel, s_InitializationStep1Set3[channel]);

    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write4(s_EmuBase, channel, s_InitializationStep1Set4[channel]);
        
    // Step 2: Wait 1024 sample periods (24 msec).
    Wait(1024);

    // Step 3: Copy the second set of initialization arrays to INIT1 through INIT4.
    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write1(s_EmuBase, channel, s_InitializationStep2Set1[channel]);

    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write2(s_EmuBase, channel, s_InitializationStep2Set2[channel]);

    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write3(s_EmuBase, channel, s_InitializationStep2Set3[channel]);

    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write4(s_EmuBase, channel, s_InitializationStep2Set4[channel]);

    // Step 4: Copy the third set of initialization arrays to INIT1 through INIT4.
    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write1(s_EmuBase, channel, s_InitializationStep3Set1[channel]);

    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write2(s_EmuBase, channel, s_InitializationStep3Set2[channel]);

    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write3(s_EmuBase, channel, s_InitializationStep3Set3[channel]);

    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write4(s_EmuBase, channel, s_InitializationStep3Set4[channel]);

    // Step 5: Set the values of registers HWCF4, HWCF5 and HWCF6 to 0, 0x00000083, and 0x00008000.
    Configuration::Write4(s_EmuBase, 0x00000000);
    Configuration::Write5(s_EmuBase, 0x00000083);
    Configuration::Write6(s_EmuBase, 0x00008000);

    // Step 6: Copy the fourth set of initialization arrays to INIT1 through INIT4.
    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write1(s_EmuBase, channel, s_InitializationStep4Set1[channel]);

    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write2(s_EmuBase, channel, s_InitializationStep4Set2[channel]);

    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write3(s_EmuBase, channel, s_InitializationStep4Set3[channel]);

    for (uint8_t channel = 0; channel < 32; ++channel)
        Initialization::Write4(s_EmuBase, channel, s_InitializationStep4Set4[channel]);

    // The arrays are now fully initialized. The output audio can now be enabled by writing 0x0004 to HWCF3.
    Configuration::Write3(s_EmuBase, 0x0004);
}

void EnableMemoryAccess()
{
    using namespace Emu8k::Voice;
    using namespace Emu8k::Global;

    for (uint8_t channel = 0; channel < 30; ++channel)
    {
        VolumeEnvelopeSustainAndDecay::Write(s_EmuBase, channel, VolumeEnvelopeSustainAndDecay::EnvelopeGeneratorDisable);
        VolumeAndFilterCutoffTargets::Write(s_EmuBase, channel, 0);
        CurrentVolumeAndFilterCutoff::Write(s_EmuBase, channel, 0);
        PanSendAndLoopStartAddress::Write(s_EmuBase, channel, 0);
        ChorusSendAndLoopEndAddress::Write(s_EmuBase, channel, 0);
        PitchTargetReverbSendAndAux::Write(s_EmuBase, channel, 0x4000 << PitchTargetReverbSendAndAux::Shift::PitchTarget);
        CurrentPitchAndFractionalAddress::Write(s_EmuBase, channel, 0x4000 << CurrentPitchAndFractionalAddress::Shift::CurrentPitch);
        QControlBitsAndCurrentAddress::Write(s_EmuBase, channel, ((channel & 1) << QControlBitsAndCurrentAddress::Shift::Access) | QControlBitsAndCurrentAddress::Dma);
    }
}

void DisableMemoryAccess()
{
    using namespace Emu8k::Voice;

    for (uint8_t channel = 0; channel < 30; ++channel)
    {
        QControlBitsAndCurrentAddress::Write(s_EmuBase, channel, ((channel & 1) << QControlBitsAndCurrentAddress::Shift::Access) | QControlBitsAndCurrentAddress::Dma);
        VolumeEnvelopeSustainAndDecay::Write(s_EmuBase, channel, VolumeEnvelopeSustainAndDecay::Release | VolumeEnvelopeSustainAndDecay::MinimumReleaseTime);
    }
}

uint32_t GetMemorySize()
{
    using namespace Emu8k::Global;
    
    uint32_t memory = 0;

    EnableMemoryAccess();

    // Set write address
    SoundMemoryAddressWrite::WriteLeft(s_EmuBase, SoundMemoryAddressWrite::DramBaseAddress);

    // Write pattern to base of memory
    SoundMemoryData::WriteLeft(s_EmuBase, 0xAA55);
    SoundMemoryData::WriteLeft(s_EmuBase, 0x3CC3);

    while (memory < 28 * 1024 * 1024)
    {
        Wait(2);

        // Set read address to check if we've been stomped
        SoundMemoryAddressRead::WriteLeft(s_EmuBase, SoundMemoryAddressWrite::DramBaseAddress);

        // Discard first word
        SoundMemoryData::ReadLeft(s_EmuBase);

        // Check first part of pattern
        if (SoundMemoryData::ReadLeft(s_EmuBase) != 0xAA55)
            break;

        // Check second part of pattern
        if (SoundMemoryData::ReadLeft(s_EmuBase) != 0x3CC3)
            break;

        //Confirmed not stomped
        memory += 32 * 1024;

        // Set write address
        SoundMemoryAddressWrite::WriteLeft(s_EmuBase, 2 * 1024 * 1024 + (memory >> 1)); //Word increment
        
        // Stomp the memory
        SoundMemoryData::WriteLeft(s_EmuBase, 0xFFFF);
        SoundMemoryData::WriteLeft(s_EmuBase, 0xFFFF);
    }

    DisableMemoryAccess();

    return memory;
}

void SetVolume(uint8_t channel, uint16_t volume)
{
    //InitialFilterCutoffAndAttenuation::Write(s_EmuBase, voice, InitialFilterCutoffAndAttenuation::NoFilter | volume);
}

void SetLinearVolume(uint8_t channel, uint16_t volume)
{

}

void SetPlaybackFrequency(uint8_t channel, uint16_t frequencyInHz)
{
    //InitialPitch::Write(s_EmuBase, voice, pitch);
}

void SetPan(uint8_t channel, uint8_t pan)
{
    using namespace Emu8k::Voice;

    PanSendAndLoopStartAddress::Write(s_EmuBase, channel, ((PanSendAndLoopStartAddress::Read(s_EmuBase, channel)) & ~PanSendAndLoopStartAddress::PanSend) | pan);
}

void PlayVoice(uint8_t channel,
               Emu8k::Global::SoundMemoryAddress_t startLocation,
               Emu8k::Global::SoundMemoryAddress_t loopStart,
               Emu8k::Global::SoundMemoryAddress_t loopEnd)
{
    using namespace Emu8k::Voice;

    VolumeEnvelopeSustainAndDecay::Write(s_EmuBase, channel, VolumeEnvelopeSustainAndDecay::Release | VolumeEnvelopeSustainAndDecay::MinimumReleaseTime);
    InitialPitch::Write(s_EmuBase, channel, InitialPitch::NoPitchShift);
    
    VolumeEnvelopeSustainAndDecay::Write(s_EmuBase, channel, VolumeEnvelopeSustainAndDecay::EnvelopeGeneratorEnable);
    VolumeAndFilterCutoffTargets::Write(s_EmuBase, channel, 0xFFFF);
    
    VolumeEnvelopeDelay::Write(s_EmuBase, channel, VolumeEnvelopeDelay::NoDelay);
    VolumeEnvelopeHoldAndAttack::Write(s_EmuBase, channel, VolumeEnvelopeHoldAndAttack::Attack6msec | VolumeEnvelopeHoldAndAttack::HoldNever);
    ModulationEnvelopeDelay::Write(s_EmuBase, channel, ModulationEnvelopeDelay::NoDelay);
    //ModulationEnvelopeHoldAndAttack::Write(s_EmuBase, channel, ModulationEnvelopeHoldAndAttack::Attack6msec | ModulationEnvelopeHoldAndAttack::HoldNever); //Already set
    ModulationEnvelopeSustainAndDecay::Write(s_EmuBase, channel, ModulationEnvelopeSustainAndDecay::Decay | ModulationEnvelopeSustainAndDecay::MinimumDecayTime);

    // LfoDelay::Write1(s_EmuBase, voice, LfoDelay::NoDelay); //Already set
    // LfoDelay::Write2(s_EmuBase, voice, LfoDelay::NoDelay); //Already set

    // PitchAndFilterEnvelopeHeights::Write(s_EmuBase, voice, 0); //Already set
    // VibratoAndFilterModulationFromLfo1::Write(s_EmuBase, voice, 0); //Already set
    // Lfo1TremoloAmountAndFrequency::Write(s_EmuBase, voice, 0); //Already set
    // Lfo2VibratoAmountAndFrequency::Write(s_EmuBase, voice, 0); //Already set

    PitchTargetReverbSendAndAux::Write(s_EmuBase, channel, (PitchTargetReverbSendAndAux::Read(s_EmuBase, channel) & ~PitchTargetReverbSendAndAux::ReverbSend) | 0);
    PanSendAndLoopStartAddress::Write(s_EmuBase, channel, ((PanSendAndLoopStartAddress::Read(s_EmuBase, channel)) & PanSendAndLoopStartAddress::PanSend) | loopStart);
    ChorusSendAndLoopEndAddress::Write(s_EmuBase, channel, loopEnd);
    QControlBitsAndCurrentAddress::Write(s_EmuBase, channel, startLocation);
    VolumeEnvelopeSustainAndDecay::Write(s_EmuBase, channel, VolumeEnvelopeSustainAndDecay::NoDecay | VolumeEnvelopeSustainAndDecay::MaximumSustain);
}

void StopVoice(uint8_t voice)
{
    using namespace Emu8k::Voice;

    VolumeEnvelopeSustainAndDecay::Write(s_EmuBase, voice, VolumeEnvelopeSustainAndDecay::Release | VolumeEnvelopeSustainAndDecay::MinimumReleaseTime);
    ModulationEnvelopeSustainAndDecay::Write(s_EmuBase, voice, ModulationEnvelopeSustainAndDecay::Release | ModulationEnvelopeSustainAndDecay::MinimumReleaseTime);
}

InitializeError_t Initialize(Has::IAllocator& allocator)
{
    if (s_Initialized)
        return InitializeError::AlreadyInitialized;

    Register_t baseAddress = 0;
    uint8_t lowDma = 0xFF;
    uint8_t highDma = 0;
    uint8_t interrupt = 0;
    Register_t mixerBase = 0;
    Register_t midiBase = 0;
    CardType_t type = CardType::Unknown;
    Register_t emuBase = 0;
    auto result = ParseEnvironmentVariable(baseAddress, lowDma, highDma, interrupt, mixerBase, midiBase, type, emuBase);
    if (result != InitializeError::Success)
        return result;

    if (DetectCard(baseAddress))
        return InitializeError::CardNotFound;

    InitializeCard(baseAddress, lowDma, highDma, interrupt, mixerBase, midiBase, type, emuBase);

    s_MemoryAvailable = GetMemorySize() - 0x40; //Don't allocate the first and last 32 bytes.

    s_Initialized = true;

    return InitializeError::Success;
}

void Shutdown()
{
    if (s_Initialized)
    {
        InitializeCard(s_BaseAddress, s_LowDma, s_HighDma, s_Interrupt, s_MixerBase, s_MidiBase, s_Type, s_EmuBase);
        s_Initialized = false;
    }
}

}