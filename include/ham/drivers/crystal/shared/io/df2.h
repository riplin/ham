//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/df2.h>

namespace Ham::Crystal::Shared::IO
{

namespace DataFormat2
{

    inline DataFormat2_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::DataFormat2);
    }

    inline void Write(Register_t baseAddress, DataFormat2_t value)
    {
        Data::Write(baseAddress, Register::DataFormat2, value);
    }

}

}