//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/misc.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        ModeAndId = Miscellaneous,
    };
}

// MODE and ID (Index 12)

// +------+-------+------+-----+------+------+------+------+
// |  D7  |  D6   |  D5  |  D4 |  D3  |  D2  |  D1  |  DO  |
// +------+-------+------+-----+------+------+------+------+
// |  1   | MODE2 | res  | res | ID3    ID2    ID1    ID0  |
// +------+-------+------+-----+------+------+------+------+

// ID3-ID0     CS4248 ID: These four bits define
//             the version of the CS4248.
//             These bits are Read-Only

// ID3-IDO = 0001 Chip version "B"
// ID3-IDO = 1010 Chip version "C" and later. See register
//           25 and the Appendix.

// MODE2       MODE 2: Enables the expanded mode
//             of the CS4231. Must be set to en-
//             able access to indirect registers
//             16-31 and their associated features.
//                 0 - MODE 1: CS4248 "look-alike".
//                 1 - MODE 2: Expanded features.

// This register's initial state after reset is: 10xx1010

typedef uint8_t ModeAndId_t;
namespace ModeAndId
{

    enum
    {
        ChipVersion = Miscellaneous::ChipVersion,
        VersionB = Miscellaneous::VersionB,
        VersionCup = Miscellaneous::VersionCup,
        Mode2Control = 0x40,
        Mode2Disable = 0x00,
        Mode2Enable = 0x40
    };

    namespace Shift
    {
        enum
        {
            ChipVersion = Miscellaneous::Shift::ChipVersion,
            Mode2Control = 0x06
        };
    };

}

}