//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <ham/drivers/amd/shared/synth/regtype.h>

namespace Ham::Amd::Shared::Synthesizer
{

namespace Register
{
    enum
    {
        VoiceSelect = 0x02
    };
}

// SVSR—Synthesizer Voice Select

// Address: P3XR+2h read/write

// Default: 00h

// Use this register to select voice-specific indirect registers for reading or writing and to enable
// auto-increment mode. The Synthesizer Voice Select register can be written with 0 through
// 31 (0h to 1Fh) to select one of 32 voices to program. Auto-increment mode allows the
// General Index register (IGIDXR) to automatically increment with every write to either of the
// general I/O data ports (I8DP and I16DP).

// +------------+------------+------------+------------+------------+------------+------------+------------+
// | Bit 7      | Bit 6      | Bit 5      | Bit 4      | Bit 3      | Bit 2      | Bit 1      | Bit 0      |
// +------------+------------+------------+------------+------------+------------+------------+------------+
// | Auto       | reserved   | reserved   | Voice Select                                                   |
// | Increment  |            |            |                                                                |
// +------------+------------+------------+------------+------------+------------+------------+------------+

// Bit 7:      Auto Increment. If set High, the value in the General Index register
//             (IGIDXR) automatically increments with every write to the General 8-Bit I/O
//             Data Port (I8DP) or the General 16-Bit I/O Data Port (I16DP). This bit is held
//             Low when the Enhanced Mode bit of the Synthesizer Global Mode register
//             (SGMI[0]) is Low.

// Bits 6–5:   Reserved.

// Bits 4–0:   Voice Select. Write a value from 0 through 31 (0h to 1Fh) to select one of
//             32 voices to program.

typedef uint8_t VoiceSelect_t;
namespace VoiceSelect
{
    enum
    {
        Voice = 0x1F,
        AutoIncrement = 0x80
    };

    inline VoiceSelect_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::VoiceSelect);
    }

    inline void Write(Register_t baseAddress, VoiceSelect_t value)
    {
        SYS_WritePortByte(baseAddress + Register::VoiceSelect, value);
    }
}

}