//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/df.h>

namespace Ham::Crystal::Shared::IO
{

namespace DataFormat
{

    inline DataFormat_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::DataFormat);
    }

    inline void Write(Register_t baseAddress, DataFormat_t value)
    {
        Data::Write(baseAddress, Register::DataFormat, value);
    }

}

}