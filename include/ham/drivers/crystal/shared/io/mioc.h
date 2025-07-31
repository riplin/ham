//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/mioc.h>

namespace Ham::Crystal::Shared::IO
{

namespace MonoInputOutputControl
{

    inline MonoInputOutputControl_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::MonoInputOutputControl);
    }

    inline void Write(Register_t baseAddress, MonoInputOutputControl_t value)
    {
        Data::Write(baseAddress, Register::MonoInputOutputControl, value);
    }
    
}

}