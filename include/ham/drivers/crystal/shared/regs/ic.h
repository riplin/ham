//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        LeftInputControl = 0x00,
        RightInputControl = 0x01
    };
}

// Left Input Control (Index O)

// +------+------+------+-----+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4 |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+-----+------+------+------+------+
// | LSS1   LSS0 | LMGE | res | LIG3   LIG2   LIG1   LIG0 |
// +------+------+------+-----+------+------+------+------+

// LIG3-LIGO   Left input gain select. The least
//             significant bit of this gain select
//             represents 1.5 dB: See Table 4.

// LMGE        Left Input Mic Gain Enable: This bit
//             will enable the 20 dB gain of the left
//             mic input signal.

// LSS1-LSSO   Left input source select. These bits
//             select the input source for the left gain
//             stage going to the left ADC.
//                 0 - Left Line
//                 1 - Left Auxiliary 1
//                 2 - Left Microphone
//                 3 - Left Line Output Loopback

// This register's initial state after reset is: 000x0000


// Right Input Control (Index 1)

// +------+------+------+-----+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4 |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+-----+------+------+------+------+
// | RSS1   RSS0 | RMGE | res | RIG3   RIG2   RIG1   RIG0 |
// +------+------+------+-----+------+------+------+------+

// RIG3-RIGO   Right input gain select. The least
//             significant bit of this gain select
//             represents 1.5 dB. See Table 4.

// RMGE        Right Input Mic Gain Enable: This bit
//             will enable the 20 dB gain of the right
//             mic input signal.

// RSS1-RSSO   Right input source select. These bits
//             select the input source for the right
//             channel gain stage going to the right
//             ADC.
//                 0 - Right Line
//                 1 - Right Auxiliary 1
//                 2 - Right Microphone
//                 3 - Right Line Out Loopback

// This register's initial state after reset is: 000x0000

// Table 4 - ADC Input Gain

//     IG3 IG2 IG1 IG0 Level
//  0   0   0   0   0   0.0 dB
//  1   0   0   0   1   1.5 dB
//  2   0   0   1   0   3.0 dB
//  3   0   0   1   1   4.5 dB
//  .         .          .
//  .         .          .
//  .         .          .
// 12   1   1   0   0  18.0 dB
// 13   1   1   0   1  19.5 dB
// 14   1   1   1   0  21.0 dB
// 15   1   1   1   1  22.5 dB

typedef uint8_t InputControl_t;
namespace InputControl
{

    enum
    {
        GainLevel = 0x0F,
        Gain0dB = 0x00,
        Gain22p5dB = 0x0F,
        MicrophoneGainControl = 0x20,
        MicrophoneGainDisable = 0x00,
        MicrophoneGainEnable = 0x20,
        Source = 0xC0,
        Line = 0x00,
        Auxilary1 = 0x40,
        Microphone = 0x80,
        LineOutLoopback = 0xC0
    };

    namespace Shift
    {
        enum
        {
            GainLevel = 0x00,
            GainControl = 0x05,
            Source = 0x06
        };
    };

}

}