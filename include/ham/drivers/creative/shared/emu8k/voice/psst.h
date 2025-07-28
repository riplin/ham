//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        PanSendAndLoopStartAddress = 0x06
    };
}

// Bits 31-24 of this doubleword register determine the left-right pan of the channel. to 0x0FF, where 0=extreme right , 0x0FF =extreme left.
// Range from 0

// Bits 23-0 determine the start address of the loop for the channel. Note that due to interpolator
// offset, the actual loop point is one greater than the start address.

typedef uint32_t PanSendAndLoopStartAddress_t;
namespace PanSendAndLoopStartAddress
{

    enum
    {
        LoopStartAddress = 0x00FFFFFF,
        PanSend = 0xFF000000
    };

    namespace Shift
    {
        enum
        {
            LoopStartAddress = 0x00,
            PanSend = 0x18
        };
    }

    inline PanSendAndLoopStartAddress_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read0DWord(baseAddress, Register::PanSendAndLoopStartAddress, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, PanSendAndLoopStartAddress_t value)
    {
        Data::Write0DWord(baseAddress, Register::PanSendAndLoopStartAddress, channel, value);
    }

}

}