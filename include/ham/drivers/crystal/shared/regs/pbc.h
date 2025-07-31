//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/bc.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        PlaybackUpperBaseCount = UpperBaseCount,
        PlaybackLowerBaseCount = LowerBaseCount
    };
}

// Playback Upper Base Register (Index 14)
// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | PUB7 | PUB6 | PUB5 | PUB4 | PUB3 | PUB2 | PUB1 | PUB0 |
// +------+------+------+------+------+------+------+------+

// PUB7-PUBO   Playback Upper Base: This register is
//             the upper byte which represents the
//             8 most significant bits of the 16-bit
//             Playback Base register. Reads from
//             this register return the same value
//             which was written. The Current
//             Count registers cannot be read.
//             When set for MODE 1 or SDC, this
//             register is used for both the Play-
//             back and Capture Base registers.

// This register's initial state after reset is: 0000000

// Playback Lower Base Register (Index 15)
// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | PLB7 | PLB6 | PLB5 | PLB4 | PLB3 | PLB2 | PLB1 | PLB0 |
// +------+------+------+------+------+------+------+------+

// PLB7-PLBO   Lower Base Bits: This register is the
//             lower byte which represents the 8
//             least significant bits of the 16-bit
//             Playback Base register. Reads from
//             this register return the same value
//             which was written. When set for
//             MODE 1 or SDC, this register is
//             used for both the Playback and Cap-
//             ture Base registers.

// This register's initial state after reset is: 00000000

typedef BaseCount8_t PlaybackBaseCount8_t;
typedef BaseCount16_t PlaybackBaseCount16_t;

}