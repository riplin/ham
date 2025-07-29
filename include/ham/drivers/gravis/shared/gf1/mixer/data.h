//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/mixer/address.h>

namespace Ham::Gravis::Shared::GF1::Mixer
{

namespace Register
{
    enum
    {
        Data = 0x106
    };
}

typedef IntegratedCircuitSystems::ICS2101::Data_t Data_t;
namespace Data
{
    inline void Write(Register_t baseAddress, Data_t value)
    {
        IntegratedCircuitSystems::ICS2101::Data::Write(baseAddress + Register::Data, value);
    }

    inline void Write(Register_t baseAddress, Address_t channel, Address_t reg, Data_t value)
    {
        IntegratedCircuitSystems::ICS2101::Data::Write(baseAddress + Register::Address, channel, reg, baseAddress + Register::Data, value);
    }
}

}
