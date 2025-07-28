//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        ModulationEnvelopeDelay = 0x06
    };
}

// This word register is programmed with the channel’s modulation envelope delay value at the
// beginning of the modulation envelope. A value of 0x8000 indicates no delay; values below
// 0x8000 indicate increasing delay in 725 usec units.

typedef uint16_t ModulationEnvelopeDelay_t;
namespace ModulationEnvelopeDelay
{

    enum
    {
        NoDelay = 0x8000,
        Delay725USec = 0x0001       //unit
    };

    inline ModulationEnvelopeDelay_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read1Word(baseAddress, Register::ModulationEnvelopeDelay, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, ModulationEnvelopeDelay_t value)
    {
        Data::Write1Word(baseAddress, Register::ModulationEnvelopeDelay, channel, value);
    }

}

}