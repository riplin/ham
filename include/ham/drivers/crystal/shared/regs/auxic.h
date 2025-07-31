//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        LeftAuxiliary1InputControl = 0x02,
        RightAuxiliary1InputControl = 0x03,
        LeftAuxiliary2InputControl = 0x04,
        RightAuxiliary2InputControl = 0x05
    };
}

// Left Auxiliary #1 Input Control (Index 2)

// +------+-----+-----+-------+-------+-------+-------+-------+
// |  D7  |  D6 |  D5 |  D4   |  D3   |  D2   |  D1   |  DO   |
// +------+-----+-----+-------+-------+-------+-------+-------+
// | LX1M | res | res | LX1G4   LX1G3   LX1G2   LX1G1   LX1G0 |
// +------+-----+-----+-------+-------+-------+-------+-------+

// LX1G4-LX1GO     Left Auxiliary #1 Mix Gain Select. The
//                 least significant bit of this gain select
//                 represents 1.5 dB. See Table 5.

// LX1M            Left Auxiliary #1 Mute. This bit, when
//                 set to 1, will mute the left channel of
//                 the Auxiliary #1 input source.

// This register's initial state after reset is: 1xx01000.


// Right Auxiliary #1 Input Control (Index 3)

// +------+-----+-----+-------+-------+-------+-------+-------+
// |  D7  |  D6 |  D5 |  D4   |  D3   |  D2   |  D1   |  DO   |
// +------+-----+-----+-------+-------+-------+-------+-------+
// | RX1M | res | res | RX1G4   RX1G3   RX1G2   RX1G1   RX1G0 |
// +------+-----+-----+-------+-------+-------+-------+-------+

// RX1G4-RX1G0     Right Auxiliary #1 Mix Gain Select.
//                 The least significant bit of this gain
//                 select represents 1.5 dB.
//                 See Table 5.

// RX1M            Right Auxiliary #1 Mute. This bit, when
//                 set to 1, will mute the right channel of
//                 the Auxiliary #1 input source.

// This register's initial state after reset is: 1xx01000.


// Left Auxiliary #2 Input Control (Index 4)

// +------+-----+-----+-------+-------+-------+-------+-------+
// |  D7  |  D6 |  D5 |  D4   |  D3   |  D2   |  D1   |  DO   |
// +------+-----+-----+-------+-------+-------+-------+-------+
// | LX2M | res | res | LX2G4   LX2G3   LX2G2   LX2G1   LX2G0 |
// +------+-----+-----+-------+-------+-------+-------+-------+

// LX2G4-LX2GO     Left Auxiliary #2 Mix Gain Select. The
//                 least significant bit of this gain select
//                 represents 1.5 dB. See Table 5.
// LX2M            Left Auxiliary #2 Mute. This bit, when
//                 set to 1, will mute the left channel of
//                 the Auxiliary #2 input source.

// This register's initial state after reset is: 1xx01000.


// Right Auxiliary #2 Input Control (Index 5)

// +------+-----+-----+-------+-------+-------+-------+-------+
// |  D7  |  D6 |  D5 |  D4   |  D3   |  D2   |  D1   |  DO   |
// +------+-----+-----+-------+-------+-------+-------+-------+
// | RX2M | res | res | RX2G4   RX2G3   RX2G2   RX2G1   RX2G0 |
// +------+-----+-----+-------+-------+-------+-------+-------+

// RX2G4-RX2GO     Right Auxiliary #2 Mix Gain Select.
//                 The least significant bit of this gain
//                 select represents 1.5 dB.
//                 See Table 5.

// RX2M            Right Auxiliary #2 Mute. This bit, when
//                 set to 1, will mute the right channel of
//                 the Auxiliary #2 input source.

// This register's initial state after reset is: 1xx01000.

// Table 5. AUX1, AUX2 Output Mix Gain

//     XxG4 XxG3 XxG2 XxG1 XxG0    Level
//  0   0    0    0    0    0       12.0 dB
//  1   0    0    0    0    1       10.5 dB
//  2   0    0    0    1    0        9.0 dB
//  3   0    0    0    1    1        7.5 dB
//  4   0    0    1    0    0        6.0 dB
//  5   0    0    1    0    1        4.5 dB
//  6   0    0    1    1    0        3.0 dB
//  7   0    0    1    1    1        1.5 dB
//  8   0    1    0    0    0        0.0 dB
//  9   0    1    0    0    1       -1.5 dB
// 10   0    1    0    1    0       -3.0 dB
// 11   0    1    0    1    1       -4.5 dB
// 12   0    1    1    0    0       -6.0 dB
//  .             .                   .
//  .             .                   .
//  .             .                   .
// 24   1    1    0    0    0      -24.0 dB
// 25   1    1    0    0    1      -25.5 dB
// 26   1    1    0    1    0      -27.0 dB
// 27   1    1    0    1    1      -28.5 dB
// 28   1    1    1    0    0      -30.0 dB
// 29   1    1    1    0    1      -31.5 dB
// 30   1    1    1    1    0      -33.0 dB
// 31   1    1    1    1    1      -34.5 dB

typedef uint8_t AuxiliaryInputControl_t;
namespace AuxiliaryInputControl
{

    enum
    {
        GainSelect = 0x1F,
        Mute = 0x80
    };

    namespace Shift
    {
        enum
        {
            GainSelect = 0x00,
            Mute = 0x07
        };
    };

}

}