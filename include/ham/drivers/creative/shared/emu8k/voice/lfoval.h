//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        Lfo1Delay = 0x05,
        Lfo2Delay = 0x07
    };
}

// This word register is programmed with the channel’s LFO #1 delay value at the beginning of the
// note. A value of 0x8000 indicates no delay; values below 0x8000 indicate increasing delay in
// 725 usec units.

typedef uint16_t LfoDelay_t;
namespace LfoDelay
{

    enum
    {
        NoDelay = 0x8000,
        Delay725USec = 0x0001       //unit
    };

    inline LfoDelay_t Read1(Register_t baseAddress, Index_t channel)
    {
        return Data::Read2Word(baseAddress, Register::Lfo1Delay, channel);
    }

    inline LfoDelay_t Read2(Register_t baseAddress, Index_t channel)
    {
        return Data::Read2Word(baseAddress, Register::Lfo2Delay, channel);
    }

    inline void Write1(Register_t baseAddress, Index_t channel, LfoDelay_t value)
    {
        Data::Write2Word(baseAddress, Register::Lfo1Delay, channel, value);
    }

    inline void Write2(Register_t baseAddress, Index_t channel, LfoDelay_t value)
    {
        Data::Write2Word(baseAddress, Register::Lfo2Delay, channel, value);
    }

}

}