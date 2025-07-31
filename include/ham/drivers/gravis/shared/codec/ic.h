//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/gravis/shared/codec/data.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::InputControl_t InputControl_t;
namespace InputControl
{
    using namespace Crystal::CS4231::InputControl;

    inline InputControl_t ReadLeft(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::LeftInputControl);
    }

    inline InputControl_t ReadRight(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::RightInputControl);
    }

    inline void WriteLeft(Register_t baseAddress, InputControl_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::LeftInputControl, value);
    }

    inline void WriteRight(Register_t baseAddress, InputControl_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::RightInputControl, value);
    }
    
}

}