//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/amd/shared/codec/regtype.h>

namespace Ham::Amd::Shared::Codec
{

typedef Crystal::CS4231::AuxiliaryInputControl_t SynthesizerInputControl_t;
namespace SynthesizerInputControl
{
    using namespace Crystal::CS4231::AuxiliaryInputControl;
}

namespace IO::SynthesizerInputControl
{
    inline SynthesizerInputControl_t ReadLeft(Register_t baseAddress)
    {
        return Crystal::CS4231::IO::AuxiliaryInputControl::ReadLeft1(baseAddress);
    }

    inline SynthesizerInputControl_t ReadRight(Register_t baseAddress)
    {
        return Crystal::CS4231::IO::AuxiliaryInputControl::ReadRight1(baseAddress);
    }
    
    inline void WriteLeft(Register_t baseAddress, SynthesizerInputControl_t value)
    {
        Crystal::CS4231::IO::AuxiliaryInputControl::WriteLeft1(baseAddress, value);
    }

    inline void WriteRight(Register_t baseAddress, SynthesizerInputControl_t value)
    {
        Crystal::CS4231::IO::AuxiliaryInputControl::WriteRight1(baseAddress, value);
    }
}

}