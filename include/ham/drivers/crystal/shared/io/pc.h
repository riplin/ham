//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/pc.h>

namespace Ham::Crystal::Shared::IO
{

namespace PinControl
{

    inline PinControl_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::PinControl);
    }

    inline void Write(Register_t baseAddress, PinControl_t value)
    {
        Data::Write(baseAddress, Register::PinControl, value);
    }

}

}