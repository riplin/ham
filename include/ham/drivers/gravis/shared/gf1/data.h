//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/index.h>

// Global Data Low - 3X4
// This register can be used to do either a 16 bit transfer for one of the 16 bit wide GF1 registers (Start addr high etc)
// when using a 16 bit I/O instruction or the low part of a 16 bit wide register when using an 8 bit I/O instruction.

// Global Data High - 3X5
// This register is used to do either an 8 bit transfer for one of the GF1 8 bit registers or to do the high part of
// a 16 bit wide register.

namespace Ham::Gravis::Shared::GF1
{

namespace Register
{
    enum
    {
        DataLow = 0x104,
        DataHigh = 0x105
    };
}

namespace Data
{

    inline Indexed::Register8_t Read8Low(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::DataLow);
    }

    inline Indexed::Register8_t Read8High(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::DataHigh);
    }

    inline Indexed::Register16_t Read16(Register_t baseAddress)
    {
        return SYS_ReadPortShort(baseAddress + Register::DataLow);
    }

    inline Indexed::Register8_t Read8Low(Register_t baseAddress, Index_t index)
    {
        Index::Write(baseAddress, index);
        return SYS_ReadPortByte(baseAddress + Register::DataLow);
    }

    inline Indexed::Register8_t Read8High(Register_t baseAddress, Index_t index)
    {
        Index::Write(baseAddress, index);
        return SYS_ReadPortByte(baseAddress + Register::DataHigh);
    }

    inline Indexed::Register16_t Read16(Register_t baseAddress, Index_t index)
    {
        Index::Write(baseAddress, index);
        return SYS_ReadPortShort(baseAddress + Register::DataLow);
    }

    inline void Write8Low(Register_t baseAddress, Index_t value)
    {
        SYS_WritePortByte(baseAddress + Register::DataLow, value);
    }

    inline void Write8High(Register_t baseAddress, Index_t value)
    {
        SYS_WritePortByte(baseAddress + Register::DataHigh, value);
    }

    inline void Write16(Register_t baseAddress, Index_t value)
    {
        SYS_WritePortShort(baseAddress + Register::DataLow, value);
    }

    inline void Write8Low(Register_t baseAddress, Index_t index, Indexed::Register8_t value)
    {
        Index::Write(baseAddress, index);
        SYS_WritePortByte(baseAddress + Register::DataLow, value);
    }

    inline void Write8High(Register_t baseAddress, Index_t index, Indexed::Register8_t value)
    {
        Index::Write(baseAddress, index);
        SYS_WritePortByte(baseAddress + Register::DataHigh, value);
    }

    inline void Write16(Register_t baseAddress, Index_t index, Indexed::Register16_t value)
    {
        Index::Write(baseAddress, index);
        SYS_WritePortShort(baseAddress + Register::DataLow, value);
    }

}

}
