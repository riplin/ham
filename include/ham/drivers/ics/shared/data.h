//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <ham/drivers/ics/shared/address.h>

namespace Ham::IntegratedCircuitSystems::Shared
{

typedef uint8_t Data_t;
namespace Data
{

    inline void Write(Register_t dataPort, Data_t value)
    {
        SYS_WritePortByte(dataPort, value);
    }

    inline void Write(Register_t addressPort, Address_t channel, Address_t reg, Register_t dataPort, Data_t value)
    {
        Address::Write(addressPort, channel, reg);
        Write(dataPort, value);
    }

}

}