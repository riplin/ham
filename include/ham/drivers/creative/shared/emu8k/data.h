//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/index.h>

namespace Ham::Creative::Shared::Emu8k
{

namespace Register
{
    enum
    {
        Data0 = 0x000,
        Data1 = 0x400,
        Data2 = 0x402,
        Data3 = 0x800
    };
}

typedef uint16_t Data16_t;
typedef uint32_t Data32_t;
namespace Data
{

    inline Data32_t Read0DWord(Register_t baseAddress)
    {
        Data16_t lo = SYS_ReadPortShort(baseAddress + Register::Data0);
        Data16_t hi = SYS_ReadPortShort(baseAddress + Register::Data0 + 2);
        return (Data32_t(hi) << 16) | lo;
    }

    inline Data32_t Read0DWord(Register_t baseAddress, uint8_t reg, Index_t channel)
    {
        Index::Write(baseAddress, (reg << Index::Shift::Register) | channel);
        return Data::Read0DWord(baseAddress);
    }

    inline Data16_t Read1Word(Register_t baseAddress)
    {
        return SYS_ReadPortShort(baseAddress + Register::Data1);
    }

    inline Data16_t Read1Word(Register_t baseAddress, uint8_t reg, Index_t channel)
    {
        Index::Write(baseAddress, (reg << Index::Shift::Register) | channel);
        return Read1Word(baseAddress);
    }

    inline Data32_t Read1DWord(Register_t baseAddress)
    {
        Data16_t lo = SYS_ReadPortShort(baseAddress + Register::Data1);
        Data16_t hi = SYS_ReadPortShort(baseAddress + Register::Data1 + 2);
        return (Data32_t(hi) << 16) | lo;
    }

    inline Data32_t Read1DWord(Register_t baseAddress, uint8_t reg, Index_t channel)
    {
        Index::Write(baseAddress, (reg << Index::Shift::Register) | channel);
        return Read1DWord(baseAddress);
    }

    inline Data16_t Read2Word(Register_t baseAddress)
    {
        return SYS_ReadPortShort(baseAddress + Register::Data2);
    }

    inline Data16_t Read2Word(Register_t baseAddress, uint8_t reg, Index_t channel)
    {
        Index::Write(baseAddress, (reg << Index::Shift::Register) | channel);
        return Read2Word(baseAddress);
    }
    
    inline Data16_t Read3Word(Register_t baseAddress)
    {
        return SYS_ReadPortShort(baseAddress + Register::Data3);
    }

    inline Data16_t Read3Word(Register_t baseAddress, uint8_t reg, Index_t channel)
    {
        Index::Write(baseAddress, (reg << Index::Shift::Register) | channel);
        return Read3Word(baseAddress);
    }

    inline void Write0DWord(Register_t baseAddress, Data32_t value)
    {
        SYS_WritePortShort(baseAddress + Register::Data0, Data16_t(value));
        SYS_WritePortShort(baseAddress + Register::Data0 + 2, Data16_t(value >> 16));
    }

    inline void Write0DWord(Register_t baseAddress, uint8_t reg, Index_t channel, Data32_t value)
    {
        Index::Write(baseAddress, (reg << Index::Shift::Register) | channel);
        Write0DWord(baseAddress, value);
    }

    inline void Write1Word(Register_t baseAddress, Data16_t value)
    {
        SYS_WritePortShort(baseAddress + Register::Data1, value);
    }

    inline void Write1Word(Register_t baseAddress, uint8_t reg, Index_t channel, Data16_t value)
    {
        Index::Write(baseAddress, (reg << Index::Shift::Register) | channel);
        Write1Word(baseAddress, value);
    }

    inline void Write1DWord(Register_t baseAddress, Data32_t value)
    {
        SYS_WritePortShort(baseAddress + Register::Data1, Data16_t(value));
        SYS_WritePortShort(baseAddress + Register::Data1 + 2, Data16_t(value >> 16));
    }

    inline void Write1DWord(Register_t baseAddress, uint8_t reg, Index_t channel, Data32_t value)
    {
        Index::Write(baseAddress, (reg << Index::Shift::Register) | channel);
        Write1DWord(baseAddress, value);
    }

    inline void Write2Word(Register_t baseAddress, Data16_t value)
    {
        SYS_WritePortShort(baseAddress + Register::Data2, value);
    }

    inline void Write2Word(Register_t baseAddress, uint8_t reg, Index_t channel, Data16_t value)
    {
        Index::Write(baseAddress, (reg << Index::Shift::Register) | channel);
        Write2Word(baseAddress, value);
    }

    inline void Write3Word(Register_t baseAddress, Data16_t value)
    {
        SYS_WritePortShort(baseAddress + Register::Data3, value);
    }

    inline void Write3Word(Register_t baseAddress, uint8_t reg, Index_t channel, Data16_t value)
    {
        Index::Write(baseAddress, (reg << Index::Shift::Register) | channel);
        Write3Word(baseAddress, value);
    }

}

}