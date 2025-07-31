//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/lic.h>

namespace Ham::Crystal::Shared::IO
{

namespace LineInputControl
{

    inline LineInputControl_t ReadLeft(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::LeftLineInputControl);
    }

    inline LineInputControl_t ReadRight(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::RightLineInputControl);
    }

    inline void WriteLeft(Register_t baseAddress, LineInputControl_t value)
    {
        Data::Write(baseAddress, Register::LeftLineInputControl, value);
    }

    inline void WriteRight(Register_t baseAddress, LineInputControl_t value)
    {
        Data::Write(baseAddress, Register::RightLineInputControl, value);
    }
    
}

}