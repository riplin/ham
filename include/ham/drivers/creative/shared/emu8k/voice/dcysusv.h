//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        VolumeEnvelopeSustainAndDecay = 0x05
    };
}

// Bit 15 of this word register determines if the values written to this channel register are decay (0)
// or release (1).

// Bits 14-8 are the volume envelope sustain level in 0.75dB increments, with 0x7f being no
// attenuation and 0 being silence.

// Bit 7 indicates that this channel’s envelope generator is to be turned off; this prevents the
// envelope engine from updating the channel’s envelopes and LFO’s, and also from writing to the
// channel’s pitch, volume, and filter target registers.

// Bits 6-0 are an encoded volume envelope’s decay or release rate, with 0x7f being the mimimum
// time of 240 usec/dB, 0x01 being the maximum time of 470 msec/dB, and 0x00 being no decay.

typedef uint16_t VolumeEnvelopeSustainAndDecay_t;
namespace VolumeEnvelopeSustainAndDecay
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
        EnvelopeGeneratorControl = 0x0080,
        EnvelopeGeneratorEnable = 0x0000,
        EnvelopeGeneratorDisable = 0x0080,
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
            DecayOrReleaseRate = 0x00,
            EnvelopeGeneratorControl = 0x07,
            SustainLevel = 0x08,
            Type = 0xF,
        };
    }

    inline VolumeEnvelopeSustainAndDecay_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read1Word(baseAddress, Register::VolumeEnvelopeSustainAndDecay, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, VolumeEnvelopeSustainAndDecay_t value)
    {
        Data::Write1Word(baseAddress, Register::VolumeEnvelopeSustainAndDecay, channel, value);
    }

}

}