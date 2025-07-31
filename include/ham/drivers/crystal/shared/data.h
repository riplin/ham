//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/index.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        Data = 0x01
    };
}

// During CS4248 initialization, this register CAN-
// NOT be written and is always read
// 10000000 (80h)

typedef uint8_t Data_t;
namespace Data
{

    inline Data_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::Data);
    }

    inline Data_t Read(Register_t baseAddress, Index_t index)
    {
        Index::Write(baseAddress, index);
        return Read(baseAddress);
    }

    inline void Write(Register_t baseAddress, Data_t value)
    {
        SYS_WritePortByte(baseAddress + Register::Data, value);
    }

    inline void Write(Register_t baseAddress, Index_t index, Data_t value)
    {
        Index::Write(baseAddress, index);
        Write(baseAddress, value);
    }

}
}