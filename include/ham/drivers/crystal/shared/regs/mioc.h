//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        MonoInputOutputControl = 0x1A
    };
}

// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | MIM  | MOM  | res  | res  | MIA3   MIA2   MIA1   MIA0 |
// +------+------+------+------+------+------+------+------+

// MIA3-MIAO   Mono Input Attenuation. MIAO is the
//             least significant bit and represents
//             3 dB attenuation, with 0000 = 0 dB.
//             See Table 7.

// MOM         Mono Output Mute. The MOM bit wil
//             mute the mono mix output, MOUT.
//             This mute is independent of the line
//             output mute.
//                 0 - no mute
//                 1 - mute

// MIM         Mono Input Mute. This bit controls the
//             mute function on the mono input,
//             MIN. The mono input provides mix
//             for the "beeper" function in most per-
//             sonal computers.
//                 0 - no mute
//                 1 - muted

// This register's initial state after reset is: 00xx0011.

typedef uint8_t MonoInputOutputControl_t;
namespace MonoInputOutputControl
{

    enum
    {
        InputAttenuation = 0x0F,
        OutputMute = 0x40,
        InputMute = 0x80
    };

    namespace Shift
    {
        enum
        {
            InputAttenuation = 0x00,
            OutputMute = 0x06,
            InputMute = 0x07
        };
    };

}

}