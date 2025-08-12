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
        StartAddressHigh = 0x02,
        StartAddressLow = 0x03,
    };
}

// Synthesizer Starting Address Registers

// The integer portion of this pair of registers specifies a real local memory boundary address
// when a voice is moving through wavetable data. These registers' value is less than the
// Synthesizer Address End registers' value. Bits 21–20 have been added to the GUS address
// to allow a voice to access 4 Mbytes of local memory instead of just 1 Mbyte. When the
// Enhanced Mode bit of the Synthesizer Global Mode register (SGMI[0]) is Low, address bits
// 21–20 are held Low.

// SASHI—Synthesizer Address Start High

// Address:    P3XR+(4-5)h read/write; index IGIDXR=02h write or IGIDXR=82h read;
//             voice index SVSR=(00h through 1Fh)

// Default: 0000h

// +------------+------------+------------+----...-----+------------+------------+------------+------------+
// | Bit 15     | Bit 14     | Bit 13     |            | Bit 3      | Bit 2      | Bit 1      | Bit 0      |
// +------------+------------+------------+----...-----+------------+------------+------------+------------+
// | reserved   | Start Address Bits 21-7                                                                  |
// |            |                                                                                          |
// +------------+------------+------------+----...-----+------------+------------+------------+------------+

// Bit 15:         Reserved.

// Bits 14–13:     Start Address Bits 21–20. Extended integer portion of Start Address.

// Bits 12–0:      Start Address Bits 19–7. Part of integer portion of Start Address.


// SASLI—Synthesizer Address Start Low

// Address: P3XR+(4-5) read/write; index IGIDXR=03h write or IGIDXR=83h read; voice

// index SVSR=(00h through 1Fh)

// Default: 0000h

// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
// | Bit 15 | Bit 14 | Bit 13 | Bit 12 | Bit 11 | Bit 10 | Bit 9 | Bit 8 | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
// | Start Address Bits 6–0                                      | Start Address (Fraction)      | reserved                              |
// |                                                             | Bits 3–0                      |                                       |
// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+

// Bits 15–9:  Start Address Bits 6–0. Part of integer portion of Start Address.

// Bits 8–5:   Start Address (Fraction) Bits 3–0. These four bits represent the upper bits
//             of a 10-bit fractional portion that is fully represented in the Synthesizer
//             Frequency Control register (SFCI).

// Bits 4–0: Reserved.

typedef Indexed::Register32_t StartAddress_t;
namespace StartAddress
{

    enum
    {
        Fraction = 0x000001E0,
        Bita6_0 = 0x0000FE00,
        Bits21_7 = 0x7FFF0000
    };

    namespace Shift
    {
        enum
        {
            Fraction = 0x05,
            Bits6_0 = 0x09,
            Bits21_7 = 0x10
        };
    }

    inline StartAddress_t Read(Register_t baseAddress)
    {
        return (StartAddress_t(Data::Read16(baseAddress, uint8_t(Register::StartAddressHigh) + uint8_t(Register::BaseRead))) << 16) |
                Data::Read16(baseAddress, uint8_t(Register::StartAddressLow) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, StartAddress_t value)
    {
        Data::Write16(baseAddress, uint8_t(Register::StartAddressHigh) + uint8_t(Voice::Register::BaseWrite), Indexed::Register16_t(value >> 16));
        Data::Write16(baseAddress, uint8_t(Register::StartAddressLow) + uint8_t(Voice::Register::BaseWrite), Indexed::Register16_t(value));
    }

}

}