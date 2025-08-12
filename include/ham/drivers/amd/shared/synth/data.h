//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/amd/shared/synth/index.h>

namespace Ham::Amd::Shared::Synthesizer
{

namespace Register
{
    enum
    {
        DataLow = 0x04,
        DataHigh = 0x05
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
