//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/mixer/data.h>

namespace Ham::Gravis::Shared::GF1::Mixer
{

typedef IntegratedCircuitSystems::ICS2101::ControlRight_t ControlRight_t;
namespace ControlRight
{
    using namespace IntegratedCircuitSystems::ICS2101::ControlRight;

    inline void Write(Register_t baseAddress, Address_t channel, ControlRight_t value)
    {
        Data::Write(baseAddress, channel, Address::ControlRight, value);
    }
}

}
