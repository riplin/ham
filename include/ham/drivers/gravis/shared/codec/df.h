//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/gravis/shared/codec/data.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::DataFormat_t DataFormat_t;
namespace DataFormat
{
    using namespace Crystal::CS4231::DataFormat;

    inline DataFormat_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::DataFormat);
    }

    inline void Write(Register_t baseAddress, DataFormat_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::DataFormat, value);
    }

}

}