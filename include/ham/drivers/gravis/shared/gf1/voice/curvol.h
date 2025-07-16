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
        CurrentVolume = 0x09
    };
}



typedef Indexed::Register16_t CurrentVolume_t;
namespace CurrentVolume
{

    enum
    {
        Mantissa = 0x0ff0,
        Exponent = 0xf000
    };

    namespace Shift
    {
        enum
        {
            Mantissa = 0x04,
            Exponent = 0x0c
        };
    }

    inline CurrentVolume_t Read(Register_t baseAddress)
    {
        return Data::Read16(baseAddress, uint8_t(Register::CurrentVolume) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, CurrentVolume_t value)
    {
        Data::Write16(baseAddress, uint8_t(Register::CurrentVolume) + uint8_t(Register::BaseWrite), value);
    }

}

}