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
        FrequencyControl = 0x01,
    };
}

// SFCI—Synthesizer Frequency Control

// Address:    P3XR+(4-5)h read/write; index IGIDXR=01h write or IGIDXR=81h read;
//             voice index SVSR=(00h through 1Fh)

// Default: 0400h

// This register controls the rate at which a voice moves through local memory addresses.
// This sets the pitch of the voice. At the default value of decimal 1.0, the synthesizer plays
// back the wavetable data at the same rate as it was recorded. Bit 0 of the fractional portion
// has been added to increase the fractional frequency resolution to 10 bits. When the
// Enhanced Mode bit of the Synthesizer Global Mode register (SGMI[0]) is Low, fractional
// portion bit 0 is held Low.

// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
// | Bit 15 | Bit 14 | Bit 13 | Bit 12 | Bit 11 | Bit 10 | Bit 9 | Bit 8 | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
// | Integer Bits 5-0                                    | Fraction Bits 9-0                                                             |
// |                                                     |                                                                               |
// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+

// Bits 15–10:     Integer Bits 5–0. Integer portion of frequency control value.

// Bits 9–1:       Fraction Bits 9–1. Fractional portion of frequency control value.

// Bit 0:          Fraction Bit 0. Fractional portion of frequency control value.


typedef Indexed::Register16_t FrequencyControl_t;
namespace FrequencyControl
{

    enum
    {
        Fraction = 0x03FF,
        Integer = 0xFC00
    };

    namespace Shift
    {
        enum
        {
            Fraction = 0x00,
            Integer = 0x0A
        };
    }

    inline FrequencyControl_t Read(Register_t baseAddress)
    {
        return Data::Read16(baseAddress, uint8_t(Register::FrequencyControl) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, FrequencyControl_t value)
    {
        Data::Write16(baseAddress, uint8_t(Register::FrequencyControl) + uint8_t(Voice::Register::BaseWrite), value);
    }

}

}