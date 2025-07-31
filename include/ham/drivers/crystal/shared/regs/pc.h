//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        PinControl = 0x0A,
    };
}

// Pin Control Register (Index 10)

// +-------+-------+------+-----+------+------+------+------+
// |  D7   |  D6   |  D5  |  D4 |  D3  |  D2  |  D1  |  DO  |
// +-------+-------+------+-----+------+------+------+------+
// | XCTL1 | XCTL0 | res  | res | DEN  | res  | IEN  | res  |
// +-------+-------+------+-----+------+------+------+------+

// IEN     Interrupt Enable: This bit enables
//         interrupts to occur on the interrupt pin.
//         The Interrupt pin will reflect the value
//         of the INT bit in the status register.
//         The interrupt pin is active high.
//             0 - Interrupt Disabled
//             1 - Interrupt Enabled
// DEN     Dither Enable: When this bit is set,
//         triangular pdf dither is added before
//         truncating the ADC 16-bit data to
//         8-bit unsigned data. Dither is only
//         active in the 8-bit unsigned data
//         format.
//             0 - Dither Disabled
//             1 - Dither Enabled
// XCTL1,  XCTL Control: These bits are
// XCTL0   reflected on the XCTL1,0 pins of the
//         CS4248
//             0 - TTL Logic Low on XCTL1,0 pins
//             1 - TTL Logic High on XCTL1,0 pins

// This registers initial state after reset is: 00xx0x0x

typedef uint8_t PinControl_t;
namespace PinControl
{

    enum
    {
        InterruptControl = 0x02,
        InterruptDisable = 0x00,
        InterruptEnable = 0x02,
        DitherControl = 0x08,
        DitherDisable = 0x00,
        DitherEnable = 0x08,
        Crystal0Control = 0x40,
        Crystal0TtlLow = 0x00,
        Crystal0TtlHigh = 0x40,
        Crystal1Control = 0x80,
        Crystal1TtlLow = 0x00,
        Crystal1TtlHigh = 0x80
    };

    namespace Shift
    {
        enum
        {
            InterruptControl = 0x02,
            DitherControl = 0x08,
            Crystal0Control = 0x40,
            Crystal1Control = 0x80
        };
    };

}

}