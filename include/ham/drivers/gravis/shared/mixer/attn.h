//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/mixer/data.h>

namespace Ham::Gravis::Shared::Mixer
{

typedef IntegratedCircuitSystems::ICS2101::Attenuation_t Attenuation_t;
namespace Attenuation
{
    using namespace IntegratedCircuitSystems::ICS2101::Attenuation;

    inline void WriteLeft(Register_t baseAddress, Address_t channel, Attenuation_t value)
    {
        Data::Write(baseAddress, channel, Address::AttenuatorLeft, value);
    }

    inline void WriteRight(Register_t baseAddress, Address_t channel, Attenuation_t value)
    {
        Data::Write(baseAddress, channel, Address::AttenuatorRight, value);
    }
}

}
