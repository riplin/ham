//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/bc.h>

namespace Ham::Crystal::Shared::IO
{

namespace BaseCount
{
    
    inline BaseCount8_t ReadUpper(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::UpperBaseCount);
    }

    inline BaseCount8_t ReadLower(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::LowerBaseCount);
    }

    inline BaseCount16_t Read(Register_t baseAddress)
    {
        return (BaseCount16_t(ReadUpper(baseAddress)) << 8) | ReadLower(baseAddress);
    }

    inline void WriteUpper(Register_t baseAddress, BaseCount8_t value)
    {
        Data::Write(baseAddress, Register::UpperBaseCount, value);
    }

    inline void WriteLower(Register_t baseAddress, BaseCount8_t value)
    {
        Data::Write(baseAddress, Register::LowerBaseCount, value);
    }

    inline void Write(Register_t baseAddress, BaseCount16_t value)
    {
        WriteUpper(baseAddress, value >> 16);
        WriteLower(baseAddress, BaseCount8_t(value));
    }

}

}