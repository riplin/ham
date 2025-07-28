//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        CurrentPitchAndFractionalAddress = 0x00
    };
}

// Bits 31-16 of this doubleword register will show the current pitch shift of the channel’s
// oscillator, with 0x4000 being no pitch shift. The current pitch is specified in linear increment.

// Bits 15-0 of the register will show the current fractional address. This register is constantly
// being overwritten with new data, so writing to it is generally pointless.

typedef uint32_t CurrentPitchAndFractionalAddress_t;
namespace CurrentPitchAndFractionalAddress
{
    enum
    {
        FractionalAddress = 0x0000FFFF,
        CurrentPitch = 0xFFFF0000
    };

    namespace Shift
    {
        enum
        {
            FractionalAddress = 0x00,
            CurrentPitch = 0x10
        };
    }

    inline CurrentPitchAndFractionalAddress_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read0DWord(baseAddress, Register::CurrentPitchAndFractionalAddress, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, CurrentPitchAndFractionalAddress_t value)
    {
        Data::Write0DWord(baseAddress, Register::CurrentPitchAndFractionalAddress, channel, value);
    }

}

}