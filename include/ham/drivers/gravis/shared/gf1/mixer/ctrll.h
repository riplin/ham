//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/mixer/data.h>

namespace Ham::Gravis::Shared::GF1::Mixer
{

typedef IntegratedCircuitSystems::ICS2101::ControlLeft_t ControlLeft_t;
namespace ControlLeft
{
    using namespace IntegratedCircuitSystems::ICS2101::ControlLeft;

    inline void Write(Register_t baseAddress, Address_t channel, ControlLeft_t value)
    {
        Data::Write(baseAddress, channel, Address::ControlLeft, value);
    }
}

}
