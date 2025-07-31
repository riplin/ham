//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        LeftOutputControl = 0x06,
        RightOutputControl = 0x07
    };
}

// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | LOM  | res  | LOA5   LOA4   LOA3   LOA2   LOA1   LOA0 |
// +------+------+------+------+------+------+------+------+

// LOAS-LOAO   Left Output Attenuate Select. The
//             least significant bit of this attenuate
//             select represents - 1.5 dB. Full
//             attenuation is at least -94.5 dB.
//             See Table 6.

// LOM         Left Output Mute. This bit, when set to
//             1, wil mute the left DAC channel
//             output.

// This register's initial state after reset is: 1x000000.


// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | ROM  | res  | ROA5   ROA4   ROA3   ROA2   ROA1   ROA0 |
// +------+------+------+------+------+------+------+------+

// ROA5-ROAO   Right Output Attenuate Select. The
//             least significant bit of this attenuate
//             select represents -1.5 dB. Full
//             attenuation must be at least -94.5 dB.
//             See Table 6.

// ROM         Right Output Mute. This bit, when set
//             to 1, will mute the right DAC output.

// This register's initial state after reset is: 1x000000.

typedef uint8_t OutputControl_t;
namespace OutputControl
{

    enum
    {
        Attenuation = 0x3F,
        Mute = 0x80
    };

    namespace Shift
    {
        enum
        {
            Attenuation = 0x00,
            Mute = 0x07
        };
    };

}

}