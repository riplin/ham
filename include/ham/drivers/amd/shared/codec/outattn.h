//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/amd/shared/codec/regtype.h>

namespace Ham::Amd::Shared::Codec
{

namespace Register
{
    enum
    {
        LeftOutputAttenuation = 0x19,
        RightOutputAttenuation = 0x1B
    };
}

// CLOAI, CROAI—Left/Right Output Attenuation

// Address:    PCODAR+1 read, write; left index CIDXR[4:0]=19h, right index
//             CIDXR[4:0]=1Bh

// Default:    1XX0 0000 (for both);

// Modes:      3 only. In mode 2, CLOAI is a read-only register that drives an 80h when read.

// This register pair controls the left and right MONO and LINE output levels. The LINE output
// mute control bit is also located in this register pair.
// Note: These registers can be written to only in mode 3, but any value written in mode
// 3 is still in effect if software changes operation to mode 1 or mode 2.

// +------------------+------------------+----------------+----------------+----------------+----------------+----------------+------------+
// | Bit 7            | Bit 6            | Bit 5          | Bit 4          | Bit 3          | Bit 2          | Bit 1          | Bit 0      |
// +------------------+------------------+----------------+----------------+----------------+----------------+----------------+------------+
// | Line Output      | reserved         | reserved       | Line Output Attenuation Select                                                 |
// | Mute Enable      |                  |                |                                                                                |
// |                  |                  |                |                                                                                |
// +------------------+------------------+----------------+----------------+----------------+----------------+----------------+------------+

// Bit 7:      Line Output Mute Enable. If set High, the LINE output is muted. If set Low,
//             the output operates normally.

// Bits 6–5:   Reserved.

// Bits 4–0:   Line Output Attenuation Select. These bits specify the amount of
//             attenuation applied to both the MONO and LINE output signals. The values
//             vary from 00h = 0 dB to 1Fh = –46.5 dB with 1.5 dB per step.

typedef Crystal::CS4231::Data_t OutputAttenuation_t;
namespace OutputAttenuation
{
    enum
    {
        Attenuation = 0x1F,
        Mute = 0x80
    };
}

namespace IO::OutputAttenuation
{
    inline OutputAttenuation_t ReadLeft(Register_t baseAddress)
    {
        return Crystal::CS4231::Data::Read(baseAddress, Register::LeftOutputAttenuation);
    }

    inline OutputAttenuation_t ReadRight(Register_t baseAddress)
    {
        return Crystal::CS4231::Data::Read(baseAddress, Register::RightOutputAttenuation);
    }
    
    inline void WriteLeft(Register_t baseAddress, OutputAttenuation_t value)
    {
        Crystal::CS4231::Data::Write(baseAddress, Register::LeftOutputAttenuation, value);
    }

    inline void WriteRight(Register_t baseAddress, OutputAttenuation_t value)
    {
        Crystal::CS4231::Data::Write(baseAddress, Register::RightOutputAttenuation, value);
    }
}

}