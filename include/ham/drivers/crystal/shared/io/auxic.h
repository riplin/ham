//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/auxic.h>

namespace Ham::Crystal::Shared::IO
{

namespace AuxiliaryInputControl
{
    inline AuxiliaryInputControl_t ReadLeft1(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::LeftAuxiliary1InputControl);
    }

    inline AuxiliaryInputControl_t ReadRight1(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::RightAuxiliary1InputControl);
    }

    inline AuxiliaryInputControl_t ReadLeft2(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::LeftAuxiliary2InputControl);
    }

    inline AuxiliaryInputControl_t ReadRight2(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::RightAuxiliary2InputControl);
    }

    inline void WriteLeft1(Register_t baseAddress, AuxiliaryInputControl_t value)
    {
        Data::Write(baseAddress, Register::LeftAuxiliary1InputControl, value);
    }

    inline void WriteRight1(Register_t baseAddress, AuxiliaryInputControl_t value)
    {
        Data::Write(baseAddress, Register::RightAuxiliary1InputControl, value);
    }

    inline void WriteLeft2(Register_t baseAddress, AuxiliaryInputControl_t value)
    {
        Data::Write(baseAddress, Register::LeftAuxiliary2InputControl, value);
    }

    inline void WriteRight2(Register_t baseAddress, AuxiliaryInputControl_t value)
    {
        Data::Write(baseAddress, Register::RightAuxiliary2InputControl, value);
    }
}

}