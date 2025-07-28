//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        ChorusSendAndLoopEndAddress = 0x07
    };
}

// Bits 31-24 of this doubleword register determine the amount of Chorus Send from the channel to
// the effects engine, with 0x00 being none and 0xff being maximum.

// Bits 23-0 determine the end address of the loop for the channel. Note that due to interpolator
// offset, the actual loop point will end at an address one greater than the loop address.

typedef uint32_t ChorusSendAndLoopEndAddress_t;
namespace ChorusSendAndLoopEndAddress
{

    enum
    {
        LoopEndAddress = 0x00FFFFFF,
        ChorusSend = 0xFF000000
    };

    namespace Shift
    {
        enum
        {
            LoopEndAddress = 0x00,
            ChorusSend = 0x18
        };
    }

    inline ChorusSendAndLoopEndAddress_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read0DWord(baseAddress, Register::ChorusSendAndLoopEndAddress, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, ChorusSendAndLoopEndAddress_t value)
    {
        Data::Write0DWord(baseAddress, Register::ChorusSendAndLoopEndAddress, channel, value);
    }

}

}