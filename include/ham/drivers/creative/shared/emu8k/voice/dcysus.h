//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        ModulationEnvelopeSustainAndDecay = 0x07
    };
}

// Bit 15 of this word register determines if the values written to this channel register are decay (0)
// or release (1).

// Bits 14-8 are the modulation envelope sustain level in 0.75dB increments, with 0x7f being no
// attenuation and 0 being zero level.

// Bit 7 should be written and read as zero.

// Bits 6-0 are an encoded modulation envelope’s decay or release rate, with 0x7f being the
// mimimum time of 240 usec/dB, 0x01 being the maximum time of 470 msec/dB, and 0x00 being
// no decay.

typedef uint16_t ModulationEnvelopeSustainAndDecay_t;
namespace ModulationEnvelopeSustainAndDecay
{

    enum
    {
        DecayOrReleaseRate = 0x007F,
        NoRelease = 0x0000,
        NoDecay = 0x0000,
        MinimumReleaseTime = 0x007F,
        MaximumReleaseTime = 0x0001,
        MinimumDecayTime = 0x007F,
        MaximumDecayTime = 0x0001,
        SustainLevel = 0x7F00,
        MaximumSustain = 0x7F00,
        Type = 0x8000,
        Decay = 0x0000,
        Release = 0x8000
    };

    namespace Shift
    {
        enum
        {
            DecayOrRelease = 0x00,
            ModulationEnvelopeSustain = 0x08,
            Type = 0x0F,
        };
    }

    inline ModulationEnvelopeSustainAndDecay_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read1Word(baseAddress, Register::ModulationEnvelopeSustainAndDecay, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, ModulationEnvelopeSustainAndDecay_t value)
    {
        Data::Write1Word(baseAddress, Register::ModulationEnvelopeSustainAndDecay, channel, value);
    }

}

}