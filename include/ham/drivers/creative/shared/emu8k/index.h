//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <ham/drivers/creative/shared/regtype.h>

namespace Ham::Creative::Shared::Emu8k
{

namespace Register
{
    enum
    {
        Index = 0x802
    };
}

typedef uint16_t Index_t;
namespace Index
{

    enum
    {
        Channel = 0x1F,
        Register = 0xE0,
    };

    namespace Shift
    {
        enum
        {
            Channel = 0x00,
            Register = 0x5
        };
    }

    inline Index_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortShort(baseAddress + Register::Index);
    }

    inline void Write(Register_t baseAddress, Index_t value)
    {
        SYS_WritePortShort(baseAddress + Register::Index, value);
    }

}

}