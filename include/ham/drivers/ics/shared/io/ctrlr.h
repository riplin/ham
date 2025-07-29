//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/ics/shared/regs/ctrlr.h>

namespace Ham::IntegratedCircuitSystems::Shared::IO
{

namespace ControlRight
{
    inline void Write(Register_t addressPort, Address_t channel, Register_t dataPort, ControlRight_t value)
    {
        Data::Write(addressPort, channel, Address::ControlRight, dataPort, value);
    }
}

}