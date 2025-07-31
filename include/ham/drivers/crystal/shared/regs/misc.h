//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        Miscellaneous = 0x0C,
    };
}

// Misc. Information Register (Index 12)

// +------+------+------+-----+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4 |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+-----+------+------+------+------+
// |  1   | res  | res  | res | ID3    ID2    ID1    ID0  |
// +------+------+------+-----+------+------+------+------+

// ID3-ID0     CS4248 ID: These four bits define
//             the version of the CS4248.
//             These bits are Read-Only

// ID3-IDO = 0001 Chip version "B"
// ID3-IDO = 1010 Chip version "C" and later.

// This register's initial state after reset is: 1xxx0001 or
// 1xxx1010 based on chip version.

typedef uint8_t Miscellaneous_t;
namespace Miscellaneous
{

    enum
    {
        ChipVersion = 0x0F,
        VersionB = 0x01,
        VersionCup = 0x0A
    };

    namespace Shift
    {
        enum
        {
            ChipVersion = 0x00
        };
    };

}

}