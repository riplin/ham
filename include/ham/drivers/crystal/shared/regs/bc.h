//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        UpperBaseCount = 0x0E,
        LowerBaseCount = 0x0F
    };
}

// Upper Base Register (Index 14)
// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | UB7  | UB6  | UB5  | UB4  | UB3  | UB2  | UB1  | UB0  |
// +------+------+------+------+------+------+------+------+

// UB7-UBO     Upper Base Bits: This byte is the
//             upper byte of the base count register.
//             It represents the 8 most significant bits
//             of the 16-bit base register. Reads from
//             this register return the same value
//             which was written. The current count
//             registers cannot be read. The base
//             register should be loaded with the buffer
//             size minus 1.

// This register's initial state after reset is: 0000000

// Lower Base Register (Index 15)
// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | LB7  | LB6  | LB5  | LB4  | LB3  | LB2  | LB1  | LB0  |
// +------+------+------+------+------+------+------+------+

// LB7-LBO     Lower Base Bits: This byte is the
//             lower byte of the base count register.
//             It represents the 8 least significant bits
//             of the 16-bit base register. Reads from
//             this register return the same value
//             which was written. The current count
//             registers cannot be read. The Base
//             register should be loaded with the
//             buffer size minus one.

// This register's initial state after reset is: 00000000

typedef uint8_t BaseCount8_t;
typedef uint16_t BaseCount16_t;

}