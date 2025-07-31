//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/gravis/shared/codec/data.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::ModeAndId_t ModeAndId_t;
namespace ModeAndId
{
    using namespace Crystal::CS4231::ModeAndId;

    inline ModeAndId_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::ModeAndId);
    }

    inline void Write(Register_t baseAddress, ModeAndId_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::ModeAndId, value);
    }

}

}