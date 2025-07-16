//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <ham/drivers/gravis/shared/regtype.h>

namespace Ham::Gravis::Shared::GF1
{

namespace Register
{
    enum
    {
        DramIO = 0x107
    };
}

// This register is used to read or write data at the location pointed at by registers 43 and 44. This is used to peek and
// poke directly to DRAM.

typedef uint8_t DramIO_t;
namespace DramIO
{

    enum
    {
        Value = 0xff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    inline DramIO_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::DramIO);
    }

    inline void Write(Register_t baseAddress, DramIO_t value)
    {
        SYS_WritePortByte(baseAddress + Register::DramIO, value);
    }

}

}
