//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/gravis/shared/regtype.h>
#include <ham/drivers/gravis/shared/codec/base.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::Capture_t Capture_t;
namespace Capture
{

    inline Capture_t Read(Register_t baseAddress)
    {
        return Crystal::CS4231::Capture::Read(baseAddress + Register::Base);
    }

}

}