//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/cdf.h>

namespace Ham::Crystal::Shared::IO
{

namespace CaptureDataFormat
{

    inline CaptureDataFormat_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::CaptureDataFormat);
    }

    inline void Write(Register_t baseAddress, CaptureDataFormat_t value)
    {
        Data::Write(baseAddress, Register::CaptureDataFormat, value);
    }

}

}