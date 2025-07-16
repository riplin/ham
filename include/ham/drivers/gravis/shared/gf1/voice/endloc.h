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
        EndLocationHigh = 0x04,
        EndLocationLow = 0x05
    };
}

// Changing the start and end points of a voice while it is playing can have some strange side effects. For example, if you
// change the end position to a lower location than it is currently playing, you will get an IRQ (if they are enabled). Also,
// since the high and low bytes are set individually and asynchronously to when the GF1 is working on a voice, it is
// possible to get an unexpected IRQ if the current position and the new end position cross.


// End location high:
// Bits 12-0   are the high 13 bits of the address of the ending location of the waveform. (Address lines 19-7)
// Bits 15-13  are not used.

// End location low:
// Bits 15-9   are the low 7 bits of the address of the ending location of the waveform. (Address lines 6-0).
// Bits 8-5    are the fractional part of the ending address.
// Bits 4-0    are not used.

typedef Indexed::Register32_t EndLocation_t;
namespace EndLocation
{

    enum
    {
        Fraction = 0x000001e0,
        Address = 0x1ffffe00
    };

    namespace Shift
    {
        enum
        {
            Fraction = 0x05,
            Address = 0x09
        };
    }

    inline EndLocation_t Read(Register_t baseAddress)
    {
        return EndLocation_t(Data::Read16(baseAddress, uint8_t(Register::EndLocationLow) + uint8_t(Register::BaseRead))) |
               (EndLocation_t(Data::Read16(baseAddress, uint8_t(Register::EndLocationHigh) + uint8_t(Register::BaseRead))) << 16);
    }

    inline void Write(Register_t baseAddress, EndLocation_t value)
    {
        Data::Write16(baseAddress, uint8_t(Register::EndLocationLow) + uint8_t(Register::BaseWrite), Indexed::Register16_t(value));
        Data::Write16(baseAddress, uint8_t(Register::EndLocationHigh) + uint8_t(Register::BaseWrite), Indexed::Register16_t(value >> 16));
    }

}

}