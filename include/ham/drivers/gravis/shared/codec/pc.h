//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/gravis/shared/codec/data.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::PinControl_t PinControl_t;
namespace PinControl
{
    using namespace Crystal::CS4231::PinControl;

    inline PinControl_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::PinControl);
    }

    inline void Write(Register_t baseAddress, PinControl_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::PinControl, value);
    }

}

}