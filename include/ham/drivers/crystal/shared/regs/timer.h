//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        TimerLowByte = 0x14,
        TimerHighByte = 0x15
    };
}

// TL7-TLO     Lower Timer Bits: This is the low order
//             byte of the 16-bit timer.

// This register's initial state after reset is: 00000000.

// TU7-TUO     Upper Timer Bits: This is the high
//             order byte of the 16-bit timer. The
//             time base is determined by the clock
//             source selected.

//             C2SL = 0 - divide XTAL1 by 245
//                    (24.576 MHz - 9.969 Ms)

//             C2SL = 1 - divide XTAL2 by 168
//                    (16.9344 MHz - 9.92 Ms)

// This register's initial state after reset is: 00000000

typedef uint8_t Timer8_t;
typedef uint16_t Timer16_t;

}