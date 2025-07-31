//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        AlternateFeatureEnableI = 0x10,
    };
}

// Alternate Feature Enable I (I16)

// +------+----+-----+------+------+------+------+------+
// |  D7  | D6 |  D5 |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+----+-----+------+------+------+------+------+
// | OLB  | TE | res | res  | res  | res  | res  | DACZ |
// +------+----+-----+------+------+------+------+------+

// DACZ    DAC Zero: This bit will force the out-
//         put of the playback channel to AC
//         zero when an underrun error occurs
//             1 - Go to center scale
//             0 - Hold previous valid sample

// TE      Timer Enable: This bit, when set, will
//         enable the timer to run and interrupt
//         the host at the specified frequency
//         in the timer registers.

// OLB     Output Level Bit: Sets the analog out-
//         put level. When clear, analog line
//         outputs are attenuated 3dB.
//             0 - Full scale of 2 Vpp (-3 dB)
//             1 - Full scale of 2.8 Vpp (0 dB)

// This register's initial state after reset is: 00xxxxx0

typedef uint8_t AlternateFeatureEnableI_t;
namespace AlternateFeatureEnableI
{

    enum
    {
        DacControl = 0x01,
        DacHoldPrevious = 0x00,
        DacCenterScale = 0x01,
        TimerControl = 0x40,
        TimerDisable = 0x00,
        TimerEnable = 0x40,
        OutputLevel = 0x80,
        OutputLevel2Vpp = 0x00,
        OutputLevel2p8Vpp = 0x80
    };

    namespace Shift
    {
        enum
        {
            DacControl = 0x00,
            TimerControl = 0x06,
            OutputLevel = 0x07,
        };
    };

}

}