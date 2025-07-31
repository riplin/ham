//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/ic.h>

namespace Ham::Crystal::Shared::IO
{

namespace InputControl
{

    inline InputControl_t ReadLeft(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::LeftInputControl);
    }

    inline InputControl_t ReadRight(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::RightInputControl);
    }

    inline void WriteLeft(Register_t baseAddress, InputControl_t value)
    {
        Data::Write(baseAddress, Register::LeftInputControl, value);
    }

    inline void WriteRight(Register_t baseAddress, InputControl_t value)
    {
        Data::Write(baseAddress, Register::RightInputControl, value);
    }
    
}

}