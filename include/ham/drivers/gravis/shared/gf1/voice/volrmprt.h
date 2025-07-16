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
        VolumeRampRate = 0x06
    };
}

// Bits 5-0    is the amount added to (or subtracted from) the current volume to get the next
//             volume. The range is from 1 to 63. The larger the number, the greater the volume step.
// Bits 7-6    defines the rate at which the increment is applied.

// Please see section 2.22 for more information on hardware volume ramping.

typedef Indexed::Register8_t VolumeRampRate_t;
namespace VolumeRampRate
{

    enum
    {
        Increment = 0x3f,
        Rate = 0xc0
    };

    namespace Shift
    {
        enum
        {
            Increment = 0x00,
            Rate = 0x06
        };
    }

    inline VolumeRampRate_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::VolumeRampRate) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, VolumeRampRate_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::VolumeRampRate) + uint8_t(Register::BaseWrite), value);
    }

}

}