//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/ics/shared/regs/balpan.h>

namespace Ham::IntegratedCircuitSystems::Shared::IO
{

namespace BalancePan
{
    inline void Write(Register_t addressPort, Address_t channel, Register_t dataPort, BalancePan_t value)
    {
        Data::Write(addressPort, channel, Address::BalancePan, dataPort, value);
    }
}

}