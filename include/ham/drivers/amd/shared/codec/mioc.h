//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/amd/shared/codec/regtype.h>

namespace Ham::Amd::Shared::Codec
{

// CMONOI—Mono Input and Output Control

// Address: PCODAR+1 read, write; index CIDXR[4:0]=1Ah

// Default: 000X 0000

// Modes: bits 7–6, 4–0 modes 2 and 3; bit 5 mode 3

// This register specifies the amount of attenuation applied to the MONO input path. The mute
// controls for the MONO input and output are also located here.    

// +---------------+----------------+----------------+----------------+----------------+----------------+----------------+----------+
// | Bit 7         | Bit 6          | Bit 5          | Bit 4          | Bit 3          | Bit 2          | Bit 1          | Bit 0    |
// +---------------+----------------+----------------+----------------+----------------+----------------+----------------+----------+
// | Mono Input    | Mono Output    | AREF to High   | reserved       | Mono Input Attenuation                                      |
// | Mute Enable   | Mute Enable    | Impedance      |                |                                                             |
// |               |                |                |                |                                                             |
// +---------------+----------------+----------------+----------------+----------------+----------------+----------------+----------+

// Bit 7:      Mono Input Mute Enable. If set High, the MONO input is muted. If set Low,
//             the input is active.

// Bit 6:      Mono Output Mute Enable. If set High, the MONO output is muted. If set
//             Low, the output operates normally.

// Bit 5:      AREF to High Impedance. If set High, the AREF pin is placed into
//             high-impedance mode. If Low, AREF operates normally. This bit is
//             accessible only in mode 3.

// Bit 4:      Reserved.

// Bits 3–0:   Mono Input Attenuation. This specifies the amount of attenuation to be
//             applied to the MONO input path. The values vary from 0 = 0 dB to
//             0Fh = –45 dB with 3.0 dB per step.

typedef Crystal::CS4231::MonoInputOutputControl_t MonoInputOutputControl_t;
namespace MonoInputOutputControl
{
    enum
    {
        InputAttenuation = Crystal::CS4231::MonoInputOutputControl::InputAttenuation,
        ArefControl = 0x20,
        ArefNormal = 0x00,
        ArefHighImpedance = 0x20,
        OutputMute = Crystal::CS4231::MonoInputOutputControl::OutputMute,
        InputMute = Crystal::CS4231::MonoInputOutputControl::InputMute
    };
}

namespace IO::MonoInputOutputControl
{
    inline MonoInputOutputControl_t Read(Register_t baseAddress)
    {
        return Crystal::CS4231::IO::MonoInputOutputControl::Read(baseAddress);
    }
    
    inline void Write(Register_t baseAddress, MonoInputOutputControl_t value)
    {
        Crystal::CS4231::IO::MonoInputOutputControl::Write(baseAddress, value);
    }
}

}