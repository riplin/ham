//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        InitialPitch = 0x00
    };
}

// This word register is programmed with the channel’s pitch shift. A value of 0xE000 indicates no
// pitch shift. Values above 0xE000 are upward pitch shift, while values below are downward
// pitch shift. The value is encoded as octaves and fractions thereof, with 0x1000 being one
// octave.

typedef uint16_t InitialPitch_t;
namespace InitialPitch
{

    enum
    {
        NoPitchShift = 0xE000,
        OneOctave = 0x1000
    };

    inline InitialPitch_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read3Word(baseAddress, Register::InitialPitch, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, InitialPitch_t value)
    {
        Data::Write3Word(baseAddress, Register::InitialPitch, channel, value);
    }

}

}