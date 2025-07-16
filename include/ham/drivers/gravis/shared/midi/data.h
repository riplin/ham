//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <ham/drivers/gravis/shared/regtype.h>

namespace Ham::Gravis::Shared::Midi
{

namespace Register
{
    enum
    {
        Data = 0x101
    };
}

// The transmit and receive registers are at 3X1 hex and are 8 bits wide.

typedef uint8_t Data_t;
namespace Data
{

    inline Data_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::Data);
    }

    inline void Write(Register_t baseAddress, Data_t value)
    {
        SYS_WritePortByte(baseAddress + Register::Data, value);
    }

}

}
