//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/ics/shared/regs/ctrll.h>

namespace Ham::IntegratedCircuitSystems::Shared::IO
{

namespace ControlLeft
{
    inline void Write(Register_t addressPort, Address_t channel, Register_t dataPort, ControlLeft_t value)
    {
        Data::Write(addressPort, channel, Address::ControlLeft, dataPort, value);
    }
}

}