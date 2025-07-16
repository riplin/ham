//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/data.h>

namespace Ham::Gravis::Shared::GF1::Global
{

namespace Register
{
    enum
    {
        SamplingFrequency = 0x48
    };
}

// The formula for calculating this value is:
// rate = 9878400/(16*(FREQ+2))

typedef Indexed::Register8_t SamplingFrequency_t;
namespace SamplingFrequency
{

    enum
    {
        Value = 0xff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    inline void Write(Register_t baseAddress, SamplingFrequency_t value)
    {
        Data::Write8High(baseAddress, Register::SamplingFrequency, value);
    }

}

}
