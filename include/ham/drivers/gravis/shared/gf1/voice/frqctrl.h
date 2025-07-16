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
        FrequencyControl = 0x01
    };
}

// Bits 15-10   - Integer Portion
// Bits 9-1     - Fractional Portion
// Bit 0        - Not used.
// This register determines the amount added to (or subtracted from) the current position of the voice to determine
// where the next position will be. This is how the interpolated data points are determined. If the FC register is less than
// 0, the GF1 will interpolate the data point in between the two actual data points. Note that the FC can be greater than
// 1. This allows for skipping over data bytes. The actual frequency that it will play back is directly related to the
// number of active voice specified (register 8E).

typedef Indexed::Register16_t FrequencyControl_t;
namespace FrequencyControl
{

    enum
    {
        Faction = 0x03fe,
        Integer = 0xfc00
    };

    namespace Shift
    {
        enum
        {
            Faction = 0x01,
            Integer = 0x0a
        };
    }

    inline FrequencyControl_t Read(Register_t baseAddress)
    {
        return Data::Read16(baseAddress, uint8_t(Register::FrequencyControl) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, FrequencyControl_t value)
    {
        Data::Write16(baseAddress, uint8_t(Register::FrequencyControl) + uint8_t(Register::BaseWrite), value);
    }

}

}