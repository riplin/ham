//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        AlternateFeatureEnableII = 0x11,
    };
}

// Alternate Feature Enable II (I17)

// +------+-----+------+------+------+------+------+------+
// |  D7  | D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+-----+------+------+------+------+------+------+
// | res  | res | res  | res  | res  | res  | res  | HPF  |
// +------+-----+------+------+------+------+------+------+

// HPF     High Pass Filter: This bit enables a
//         DC-blocking high-pass filter in the
//         digital filter of the ADC. This filter
//         forces the ADC offset of 0.
//             0 - disabled
//             1 - enabled

// This register's initial state after reset is: xxxxxxx0.

typedef uint8_t AlternateFeatureEnableII_t;
namespace AlternateFeatureEnableII
{

    enum
    {
        HighPassFilterControl = 0x01,
        HighPassFilterDisable = 0x00,
        HighPasFilterEnable = 0x01
    };

    namespace Shift
    {
        enum
        {
            HighPassFilterControl = 0x00
        };
    };

}

}