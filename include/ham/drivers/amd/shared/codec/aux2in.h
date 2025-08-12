//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/amd/shared/codec/regtype.h>

namespace Ham::Amd::Shared::Codec
{

typedef Crystal::CS4231::AuxiliaryInputControl_t Auxiliary2InputControl_t;
namespace Auxiliary2InputControl
{
    using namespace Crystal::CS4231::AuxiliaryInputControl;
}

namespace IO::Auxiliary2InputControl
{
    inline Auxiliary2InputControl_t ReadLeft(Register_t baseAddress)
    {
        return Crystal::CS4231::IO::AuxiliaryInputControl::ReadLeft2(baseAddress);
    }

    inline Auxiliary2InputControl_t ReadRight(Register_t baseAddress)
    {
        return Crystal::CS4231::IO::AuxiliaryInputControl::ReadRight2(baseAddress);
    }
    
    inline void WriteLeft(Register_t baseAddress, Auxiliary2InputControl_t value)
    {
        Crystal::CS4231::IO::AuxiliaryInputControl::WriteLeft2(baseAddress, value);
    }

    inline void WriteRight(Register_t baseAddress, Auxiliary2InputControl_t value)
    {
        Crystal::CS4231::IO::AuxiliaryInputControl::WriteRight2(baseAddress, value);
    }
}

}