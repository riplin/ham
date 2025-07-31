//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/gravis/shared/codec/data.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::AuxiliaryInputControl_t AuxiliaryInputControl_t;
namespace AuxiliaryInputControl
{
    using namespace Crystal::CS4231::AuxiliaryInputControl;

    inline AuxiliaryInputControl_t ReadLeft1(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::LeftAuxiliary1InputControl);
    }

    inline AuxiliaryInputControl_t ReadRight1(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::RightAuxiliary1InputControl);
    }

    inline AuxiliaryInputControl_t ReadLeft2(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::LeftAuxiliary2InputControl);
    }

    inline AuxiliaryInputControl_t ReadRight2(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::RightAuxiliary2InputControl);
    }

    inline void WriteLeft1(Register_t baseAddress, AuxiliaryInputControl_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::LeftAuxiliary1InputControl, value);
    }

    inline void WriteRight1(Register_t baseAddress, AuxiliaryInputControl_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::RightAuxiliary1InputControl, value);
    }

    inline void WriteLeft2(Register_t baseAddress, AuxiliaryInputControl_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::LeftAuxiliary2InputControl, value);
    }

    inline void WriteRight2(Register_t baseAddress, AuxiliaryInputControl_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::RightAuxiliary2InputControl, value);
    }
}

}