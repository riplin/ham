//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/pbc.h>

namespace Ham::Crystal::Shared::IO
{

namespace PlaybackBaseCount
{
    
    inline PlaybackBaseCount8_t ReadUpper(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::PlaybackUpperBaseCount);
    }

    inline PlaybackBaseCount8_t ReadLower(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::PlaybackLowerBaseCount);
    }

    inline PlaybackBaseCount16_t Read(Register_t baseAddress)
    {
        return (PlaybackBaseCount16_t(ReadUpper(baseAddress)) << 8) | ReadLower(baseAddress);
    }

    inline void WriteUpper(Register_t baseAddress, PlaybackBaseCount8_t value)
    {
        Data::Write(baseAddress, Register::PlaybackUpperBaseCount, value);
    }

    inline void WriteLower(Register_t baseAddress, PlaybackBaseCount8_t value)
    {
        Data::Write(baseAddress, Register::PlaybackLowerBaseCount, value);
    }

    inline void Write(Register_t baseAddress, PlaybackBaseCount16_t value)
    {
        WriteUpper(baseAddress, value >> 16);
        WriteLower(baseAddress, PlaybackBaseCount8_t(value));
    }

}

}