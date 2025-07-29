//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/mixer/data.h>

namespace Ham::Gravis::Shared::GF1::Mixer
{

typedef IntegratedCircuitSystems::ICS2101::BalancePan_t BalancePan_t;
namespace BalancePan
{
    using namespace IntegratedCircuitSystems::ICS2101::BalancePan;

    inline void Write(Register_t baseAddress, Address_t channel, BalancePan_t value)
    {
        Data::Write(baseAddress, channel, Address::BalancePan, value);
    }
}

}
