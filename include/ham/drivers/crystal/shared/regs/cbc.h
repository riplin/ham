//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        CaptureUpperBaseCount = 0x1E,
        CaptureLowerBaseCount = 0x1F
    };
}

// Capture Upper Base (I30)
// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | UB7  | UB6  | UB5  | UB4  | UB3  | UB2  | UB1  | UB0  |
// +------+------+------+------+------+------+------+------+

// CUB7-CUBO   Capture Upper Base: This register is
//             the upper byte which represents the
//             8 most significant bits of the 16-bit
//             Capture Base register. Reads from
//             this this register returns the same
//             value that was written.

// This register's initial state after reset is: 0000000



// Capture Lower Base (I31)
// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | LB7  | LB6  | LB5  | LB4  | LB3  | LB2  | LB1  | LB0  |
// +------+------+------+------+------+------+------+------+

// CLB7-CLB0   Lower Base Bits: This register is the
//             lower byte which represents the 8
//             least significant bits of the 16-bit
//             Capture Base register. Reads from
//             this register returns the same value
//             which was written.

// This register's initial state after reset is: 00000000

typedef uint8_t CaptureBaseCount8_t;
typedef uint16_t CaptureBaseCount16_t;

}