//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/ics/shared/regs/attn.h>

namespace Ham::IntegratedCircuitSystems::Shared::IO
{

namespace Attenuation
{
    inline void WriteLeft(Register_t addressPort, Address_t channel, Register_t dataPort, Attenuation_t value)
    {
        Data::Write(addressPort, channel, Address::AttenuatorLeft, dataPort, value);
    }

    inline void WriteRight(Register_t addressPort, Address_t channel, Register_t dataPort, Attenuation_t value)
    {
        Data::Write(addressPort, channel, Address::AttenuatorRight, dataPort, value);
    }
}

}