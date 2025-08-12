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
        UpperAddress = 0x10,
    };
}

// SUAI—Synthesizer Upper Address

// Address: P3XR+5h read/write; index IGIDXR=10h write or IGIDXR=90h read; voice

// index SVSR=(00h through 1Fh)

// Default: 00h

// This register contains the upper bits of the local memory address for a voice’s wavetable
// data. The upper bits of the address are added to the starting, ending, and current addresses
// for each voice. The upper address bits fix a voice in one of four 4-Mbyte memory spaces.
// With the upper address bits, the synthesizer can address a total of 16 Mbytes of memory
// for wavetable data. When the Enhanced Mode bit of the Synthesizer Global Mode register
// (SGMI[0]) is Low, SUAI is held to the default value.

// +------------+------------+------------+------------+------------+------------+------------+------------+
// | Bit 7      | Bit 6      | Bit 5      | Bit 4      | Bit 3      | Bit 2      | Bit 1      | Bit 0      |
// +------------+------------+------------+------------+------------+------------+------------+------------+
// | reserved   | reserved   | reserved   | reserved   | reserved   | reserved   | Upper address bits      |
// |            |            |            |            |            |            | 23-22                   |
// +------------+------------+------------+------------+------------+------------+------------+------------+

// Bits 7–2:   Reserved.

// Bits 1–0:   Upper Address. Bits 23–22 of the local memory addresses for a voice’s
//             wavetable data.

typedef Indexed::Register8_t UpperAddress_t;
namespace UpperAddress
{

    enum
    {
        Bits = 0x03
    };

    namespace Shift
    {
        enum
        {
            Bits = 0x00
        };
    }

    inline UpperAddress_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::UpperAddress) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, UpperAddress_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::UpperAddress) + uint8_t(Voice::Register::BaseWrite), value);
    }

}

}