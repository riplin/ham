//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/gravis/shared/regtype.h>
#include <ham/drivers/gravis/shared/codec/base.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::Play_t Play_t;
namespace Play
{

    inline void Write(Register_t baseAddress, Play_t value)
    {
        Crystal::CS4231::Play::Write(baseAddress + Register::Base, value);
    }
    
}

}