//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/gravis/shared/codec/data.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::OutputControl_t OutputControl_t;
namespace OutputControl
{
    using namespace Crystal::CS4231::OutputControl;

    inline OutputControl_t ReadLeft(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::LeftOutputControl);
    }

    inline OutputControl_t ReadRight(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::RightOutputControl);
    }

    inline void WriteLeft(Register_t baseAddress, OutputControl_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::LeftOutputControl, value);
    }

    inline void WriteRight(Register_t baseAddress, OutputControl_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::RightOutputControl, value);
    }

}
}