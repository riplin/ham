//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/oc.h>

namespace Ham::Crystal::Shared::IO
{

namespace OutputControl
{

    inline OutputControl_t ReadLeft(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::LeftOutputControl);
    }

    inline OutputControl_t ReadRight(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::RightOutputControl);
    }

    inline void WriteLeft(Register_t baseAddress, OutputControl_t value)
    {
        Data::Write(baseAddress, Register::LeftOutputControl, value);
    }

    inline void WriteRight(Register_t baseAddress, OutputControl_t value)
    {
        Data::Write(baseAddress, Register::RightOutputControl, value);
    }

}
}