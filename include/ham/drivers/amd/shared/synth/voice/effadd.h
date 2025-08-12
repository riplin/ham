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
        EffectsAddressHigh = 0x11,
        EffectsAddressLow = 0x12,
    };
}

// Synthesizer Effects Address Registers

// When a voice operates as an effects processor, this pair of registers indicates the current
// address where data is being written in local memory. The data is from the effects
// accumulator linked to the effects processor. The effects address is integer only, because
// the data is being written. The integer portion of all wavetable addresses represents a local
// memory location.

// SEAHI—Synthesizer Effects Address High

// Address:    P3XR+(4-5)h read/write; index IGIDXR=11h write or IGIDXR=91h read;
//             voice index SVSR=(00h through 1Fh)

// Default: 0000h

// +------------+------------+------------+----...-----+------------+------------+------------+------------+
// | Bit 15     | Bit 14     | Bit 13     |            | Bit 3      | Bit 2      | Bit 1      | Bit 0      |
// +------------+------------+------------+----...-----+------------+------------+------------+------------+
// | reserved   | Effects Address Bits 21-7                                                                |
// |            |                                                                                          |
// +------------+------------+------------+----...-----+------------+------------+------------+------------+

// Bit 15:     Reserved.

// Bits 14–0:  Effects Address Bits 21–7. Part of integer portion of Address.

// SEALI—Synthesizer Effects Address Low

// Address:    P3XR+(4-5)h read/write; index IGIDXR=12h write or IGIDXR=92h read;
//             voice index SVSR=(00h through 1Fh)

// Default: 0000h

// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
// | Bit 15 | Bit 14 | Bit 13 | Bit 12 | Bit 11 | Bit 10 | Bit 9 | Bit 8 | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
// | Address Bits 6–0                                            | reserved                                                              |
// |                                                             |                                                                       |
// +--------+--------+--------+--------+--------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+

// Bits 15–9:  Effects Address Bits 6–0. Part of integer portion of the Address.

// Bits 8–0:   Reserved.

// Note: All bits in this register except reserved bits are self-modifying.

typedef Indexed::Register32_t EffectsAddress_t;
namespace EffectsAddress
{

    enum
    {
        Bits6_0 = 0x0000FE00,
        Bits21_7 = 0x7FFF0000
    };

    namespace Shift
    {
        enum
        {
            Bits6_0 = 0x09,
            Bits21_7 = 0x10
        };
    }

    inline EffectsAddress_t Read(Register_t baseAddress)
    {
        return (EffectsAddress_t(Data::Read16(baseAddress, uint8_t(Register::EffectsAddressHigh) + uint8_t(Register::BaseRead))) << 16) |
                Data::Read16(baseAddress, uint8_t(Register::EffectsAddressLow) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, EffectsAddress_t value)
    {
        Data::Write16(baseAddress, uint8_t(Register::EffectsAddressHigh) + uint8_t(Voice::Register::BaseWrite), Indexed::Register16_t(value >> 16));
        Data::Write16(baseAddress, uint8_t(Register::EffectsAddressLow) + uint8_t(Voice::Register::BaseWrite), Indexed::Register16_t(value));
    }

}

}