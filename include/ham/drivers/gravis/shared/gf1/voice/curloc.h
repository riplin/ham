//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/data.h>
#include <ham/drivers/gravis/shared/gf1/voice/vocbase.h>

namespace Ham::Gravis::Shared::GF1::Voice
{

namespace Register
{
    enum
    {
        CurrentLocationHigh = 0x0a,
        CurrentLocationLow = 0x0b
    };
}

// High:
// Bits 15-13  Reserved (Set to 0)
// Bits 12-0   High 13 bits of address (address lines 19-7)

// Low:
// Bits 15-9   Low 7 bits of address. (address lines 6-0)
// Bits 8-0    9 bit fractional position.

typedef Indexed::Register32_t CurrentLocation_t;
namespace CurrentLocation
{

    enum
    {
        Fraction = 0x000001ff,
        Address = 0x1ffffe00
    };

    namespace Shift
    {
        enum
        {
            Fraction = 0x00,
            Address = 0x09
        };
    }

    inline CurrentLocation_t Read(Register_t baseAddress)
    {
        return CurrentLocation_t(Data::Read16(baseAddress, uint8_t(Register::CurrentLocationLow) + uint8_t(Register::BaseRead))) |
               (CurrentLocation_t(Data::Read16(baseAddress, uint8_t(Register::CurrentLocationHigh) + uint8_t(Register::BaseRead))) << 16);
    }

    inline void Write(Register_t baseAddress, CurrentLocation_t value)
    {
        Data::Write16(baseAddress, uint8_t(Register::CurrentLocationLow) + uint8_t(Register::BaseWrite), Indexed::Register16_t(value));
        Data::Write16(baseAddress, uint8_t(Register::CurrentLocationHigh) + uint8_t(Register::BaseWrite), Indexed::Register16_t(value >> 16));
    }

}

}