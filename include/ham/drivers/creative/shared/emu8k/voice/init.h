//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        Initialization1 = 0x02,
        Initialization2 = 0x02,
        Initialization3 = 0x03,
        Initialization4 = 0x03
    };
}

// During initialization, values will be written to this word array. After this process is complete, the
// register can be ignored.

typedef uint16_t Initialization_t;
namespace Initialization
{

    inline Initialization_t Read1(Register_t baseAddress, uint8_t channel)
    {
        return Data::Read1Word(baseAddress, Register::Initialization1, channel);
    }

    inline Initialization_t Read2(Register_t baseAddress, uint8_t channel)
    {
        return Data::Read2Word(baseAddress, Register::Initialization2, channel);
    }

    inline Initialization_t Read3(Register_t baseAddress, uint8_t channel)
    {
        return Data::Read1Word(baseAddress, Register::Initialization3, channel);
    }

    inline Initialization_t Read4(Register_t baseAddress, uint8_t channel)
    {
        return Data::Read2Word(baseAddress, Register::Initialization4, channel);
    }

    inline void Write1(Register_t baseAddress, uint8_t channel, Initialization_t value)
    {
        Data::Write1Word(baseAddress, Register::Initialization1, channel, value);
    }

    inline void Write2(Register_t baseAddress, uint8_t channel, Initialization_t value)
    {
        Data::Write2Word(baseAddress, Register::Initialization2, channel, value);
    }

    inline void Write3(Register_t baseAddress, uint8_t channel, Initialization_t value)
    {
        Data::Write1Word(baseAddress, Register::Initialization3, channel, value);
    }

    inline void Write4(Register_t baseAddress, uint8_t channel, Initialization_t value)
    {
        Data::Write2Word(baseAddress, Register::Initialization4, channel, value);
    }

}

}