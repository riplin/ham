//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/cbc.h>

namespace Ham::Crystal::Shared::IO
{

namespace CaptureBaseCount
{
    
    inline CaptureBaseCount8_t ReadUpper(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::CaptureUpperBaseCount);
    }

    inline CaptureBaseCount8_t ReadLower(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::CaptureLowerBaseCount);
    }

    inline CaptureBaseCount16_t Read(Register_t baseAddress)
    {
        return (CaptureBaseCount16_t(ReadUpper(baseAddress)) << 8) | ReadLower(baseAddress);
    }

    inline void WriteUpper(Register_t baseAddress, CaptureBaseCount8_t value)
    {
        Data::Write(baseAddress, Register::CaptureUpperBaseCount, value);
    }

    inline void WriteLower(Register_t baseAddress, CaptureBaseCount8_t value)
    {
        Data::Write(baseAddress, Register::CaptureLowerBaseCount, value);
    }

    inline void Write(Register_t baseAddress, CaptureBaseCount16_t value)
    {
        WriteUpper(baseAddress, value >> 16);
        WriteLower(baseAddress, CaptureBaseCount8_t(value));
    }

}

}