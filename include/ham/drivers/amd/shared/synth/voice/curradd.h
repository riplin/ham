//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/amd/shared/synth/data.h>
#include <ham/drivers/amd/shared/synth/voice/vocbase.h>

namespace Ham::Amd::Shared::Synthesizer::Voice
{

namespace Register
{
    enum
    {
        CurrentAddressHigh = 0x0A,
        CurrentAddressLow = 0x0B,
    };
}

// Synthesizer Current Address Registers

// The integer portion of this pair of registers is the current location in local memory where a
// voice is fetching sample data. The fractional portion is used to interpolate between the
// sample in the location addressed by the integer portion of the address (address bits 21–0)
// and the sample in the location addressed by the value in bits 21–0 plus 1. This register pair
// is self-modifying and changes values as a voice moves through wavetable data in local
// memory. Bits 21–20 have been added to the GUS address to allow a voice to access
// 4 Mbytes of local memory instead of just 1 Mbyte. When the Enhanced Mode bit of the
// Synthesizer Global Mode register (SGMI[0]) is Low, address bits 21–20 are held Low. Bit
// 0 of the fractional portion of the address is used in interpolation but is not normally accessible
// for programming. Resetting and writing to the Synthesizer Address Low register (SALI)
// clears bit 0 of the fractional portion of the address.

// SAHI—Synthesizer Address High

// Address:    P3XR+(4-5)h read/write; index IGIDXR=Ah write or IGIDXR=8Ah read;
//             voice index SVSR=(00h through 1Fh)

// Default: 0000h

// +------------+------------+------------+----...-----+------------+------------+------------+------------+
// | Bit 15     | Bit 14     | Bit 13     |            | Bit 3      | Bit 2      | Bit 1      | Bit 0      |
// +------------+------------+------------+----...-----+------------+------------+------------+------------+
// | reserved   | End Address Bits 21-7                                                                    |
// |            |                                                                                          |
// +------------+------------+------------+----...-----+------------+------------+------------+------------+

// Bit 15:         Reserved.

// Bits 14–13:     Address Bits 21–20. Extended integer portion of Address.

// Bits 12–0:      Address Bits 19–7. Part of integer portion of Address.

// Note: All bits in this register except reserved bits are self-modifying.


// SALI—Synthesizer Address Low

// Address:    P3XR+(4-5)h read/write; index IGIDXR=Bh write or IGIDXR=8Bh read;
//             voice index SVSR=(00h through 1Fh)

// Default: 0000h

// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
// | Bit 15 | Bit 14 | Bit 13 | Bit 12 | Bit 11 | Bit 10 | Bit 9 | Bit 8 | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
// | Address Bits 6–0                                            | Address (Fraction)                                                    |
// |                                                             | Bits 9–1                                                              |
// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+

// Bits 15–9:  Address Bits 6–0. Part of integer portion of the Address.

// Bits 8–0:   Address (Fraction) Bits 9–1. Fractional bits used during interpolation.

// Note: All bits in this register are self-modifying.

typedef Indexed::Register32_t EndAddress_t;
namespace EndAddress
{

    enum
    {
        Fraction = 0x000001FF,
        Bits6_0 = 0x0000FE00,
        Bits21_7 = 0x7FFF0000
    };

    namespace Shift
    {
        enum
        {
            Fraction = 0x00,
            Bits6_0 = 0x09,
            Bits21_7 = 0x10
        };
    }

    inline EndAddress_t Read(Register_t baseAddress)
    {
        return (EndAddress_t(Data::Read16(baseAddress, uint8_t(Register::CurrentAddressHigh) + uint8_t(Register::BaseRead))) << 16) |
                Data::Read16(baseAddress, uint8_t(Register::CurrentAddressLow) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, EndAddress_t value)
    {
        Data::Write16(baseAddress, uint8_t(Register::CurrentAddressHigh) + uint8_t(Voice::Register::BaseWrite), Indexed::Register16_t(value >> 16));
        Data::Write16(baseAddress, uint8_t(Register::CurrentAddressLow) + uint8_t(Voice::Register::BaseWrite), Indexed::Register16_t(value));
    }

}

}