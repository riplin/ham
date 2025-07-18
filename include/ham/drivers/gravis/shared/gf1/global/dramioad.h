//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/data.h>

namespace Ham::Gravis::Shared::GF1::Global
{

namespace Register
{
    enum
    {
        DramIOAddressLow = 0x43,
        DramIOAddressHigh = 0x44
    };
}

// These 2 registers allow you to specify an address to peek and poke directly into UltraSound DRAM. Register 43 is
// the lower 16 address lines. Register 44 is the upper 4 address lines. (bits 0-3). Read or write to register 3X7 to get at
// the address location.

typedef Indexed::Register24_t DramIOAddress_t;
namespace DramIOAddress
{

    enum
    {
        Value = 0x0fffff,
        BankSelect = 0x0c0000
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    inline void Write(Register_t baseAddress, DramIOAddress_t value)
    {
        Data::Write16(baseAddress, Register::DramIOAddressLow, Indexed::Register16_t(value));
        Data::Write8High(baseAddress, Register::DramIOAddressHigh, Indexed::Register8_t(value >> 16));
    }

}

}
