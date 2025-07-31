//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        LeftLineInputControl = 0x12,
        RightLineInputControl = 0x13
    };
}

// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | LLM  | res    res  | LLG4   LLG3   LLG2   LLG1   LLG0 |
// +------+------+------+------+------+------+------+------+

// LLG4-LLGO   Left Line, LLINE, Mix Gain. The least
//             significant bit represents 1.5 dB, with
//             01000 = 0 dB. See Table 5.

// LLM         Left Line Mute. When set to 1, the left
//             Line input, LLINE, to the mixer, is
//             muted.

// This register's initial state after reset is: 1xx01000.

// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | RLM  | res    res  | RLG4   RLG3   RLG2   RLG1   RLG0 |
// +------+------+------+------+------+------+------+------+

// RLG4-RLGO   Right Line, RLINE, Mix Gain. The least
//             significant bit represents 1.5 dB, with
//             01000 = 0 dB. See Table 5.
// RLM         Right Line Mute. When set to 1, the
//             Right Line input, RLINE, to the
//             mixer, is muted.

// This register's initial state after reset is: 1xx01000.

typedef uint8_t LineInputControl_t;
namespace LineInputControl
{

    enum
    {
        Gain = 0x1F,
        Mute = 0x80
    };

    namespace Shift
    {
        enum
        {
            Gain = 0x00,
            Mute = 0x07
        };
    };

}

}