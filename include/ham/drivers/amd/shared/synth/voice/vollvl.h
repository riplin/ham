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
        VolumeLevel = 0x09,
    };
}

// SVLI—Synthesizer Volume Level

// Address:    P3XR+(4-5)h read/write; index IGIDXR=09h write or IGIDXR=89h read;
//             voice index SVSR=(00h through 1Fh)

// Default: 0000h

// This register contains the current value of the looping component of volume. Volume has
// three fractional bits that are needed for more resolution when choosing a slow rate of
// increment. These three bits do not affect the volume multiplication until an increment causes
// them to roll over into the least significant bit (LSB) of the Looping Volume field.

// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+----------+
// | Bit 15 | Bit 14 | Bit 13 | Bit 12 | Bit 11 | Bit 10 | Bit 9 | Bit 8 | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0    |
// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+----------+
// | Looping Volume Integer Bits 11-0                                                                    | Looping Volume        | reserved |
// |                                                                                                     | Fraction Bits 2-0     |          |
// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+----------+

// Bits 15–4:  Looping Volume Integer Bits 11–0. Current looping volume value.

// Bits 3–1:   Looping Volume Fraction Bits 2–0. Fractional volume value.

// Bit 0:      Reserved.

// Note: All bits in this register except reserved bits are self-modifying.

typedef Indexed::Register16_t VolumeLevel_t;
namespace VolumeLevel
{

    enum
    {
        Fraction = 0x000E,
        Integer = 0xFFF0
    };

    namespace Shift
    {
        enum
        {
            Fraction = 0x01,
            Integer = 0x04
        };
    }

    inline VolumeLevel_t Read(Register_t baseAddress)
    {
        return Data::Read16(baseAddress, uint8_t(Register::VolumeLevel) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, VolumeLevel_t value)
    {
        Data::Write16(baseAddress, uint8_t(Register::VolumeLevel) + uint8_t(Voice::Register::BaseWrite), value);
    }

}

}