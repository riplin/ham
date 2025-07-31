//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/lc.h>

namespace Ham::Crystal::Shared::IO
{

namespace LoopbackControl
{

    inline LoopbackControl_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::LoopbackControl);
    }

    inline void Write(Register_t baseAddress, LoopbackControl_t value)
    {
        Data::Write(baseAddress, Register::LoopbackControl, value);
    }

}

}