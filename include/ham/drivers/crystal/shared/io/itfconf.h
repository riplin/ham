//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/itfconf.h>

namespace Ham::Crystal::Shared::IO
{

namespace InterfaceControl
{

    inline InterfaceControl_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::InterfaceControl);
    }

    inline void Write(Register_t baseAddress, InterfaceControl_t value)
    {
        Data::Write(baseAddress, Register::InterfaceControl, value);
    }

}

}