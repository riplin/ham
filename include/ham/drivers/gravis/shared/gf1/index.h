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
        Index = 0x103
    };
}

typedef uint8_t Index_t;
namespace Index
{

    inline Index_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::Index);
    }

    inline void Write(Register_t baseAddress, Index_t value)
    {
        SYS_WritePortByte(baseAddress + Register::Index, value);
    }

}

}
