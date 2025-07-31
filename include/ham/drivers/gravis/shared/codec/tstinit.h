//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/gravis/shared/codec/data.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::ErrorStatusAndInitialization_t ErrorStatusAndInitialization_t;
namespace ErrorStatusAndInitialization
{
    using namespace Crystal::CS4231::ErrorStatusAndInitialization;

    inline ErrorStatusAndInitialization_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::TestAndInitialization);
    }

    inline void Write(Register_t baseAddress, ErrorStatusAndInitialization_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::TestAndInitialization, value);
    }

}

}