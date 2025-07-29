//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/ics/shared/data.h>

// Modes of Operation

// Each attenuator pair may operate in one of three modes:
//     - Normal mode
//     - Stereo mode
//     - Balance/Pan mode

// Normal Mode
// Normal mode is used for applications with mono inputs. In this
// mode, both internal attenuator (left and right) and control
// registers (left and right) are utilized. Each may be controlled
// separately.

// Stereo Mode
// The operation of the attenuator pair in stereo mode is controlled
// by the left control register. The data written to the left control
// register is also written to the right control register; therefore,
// application software need not write to the right control or right
// attenuation register while operating in this mode. The gain of
// both channels will be identical in stereo mode. The master
// attenuator pair does not have a mono switch and cannot be
// operated in any mode requiring the mono switch to be in the
// closed (or on) position.

// Modes of Operation -
// Data Values and Switch Settings

// Left Control Register
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | D6 | D5 | D4 | D3 | D2 | D1 | D0 || LL | LR | RL | RR | Mono | Mode of Operation  |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | XX | XX | XX |  0 |  0 |  0 |  0 || Off| Off| XX | XX | Off  | Normal             |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | XX | XX | XX |  0 |  0 |  0 |  1 || On | Off| XX | XX | Off  | Normal             |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | XX | XX | XX |  0 |  0 |  1 |  0 || Off| On | XX | XX | Off  | Normal             |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | XX | XX | XX |  0 |  0 |  1 |  1 || On | On | XX | XX | Off  | Normal             |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | XX | XX | XX |  0 |  1 |  0 |  0 || On | Off| Off| On | Off  | Stereo - Normal    |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | XX | XX | XX |  0 |  1 |  0 |  1 || Off| On | On | Off| Off  | Stereo - Reversed  |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | XX | XX | XX |  0 |  1 |  1 |  0 || On | Off| Off| On | On   | Mono               |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | XX | XX | XX |  0 |  1 |  1 |  1 || Off| On | On | Off| On   | Mono (Reserved)    |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | XX | XX | XX |  1 |  0 |  0 |  0 || On | Off| Off| On | Off  | Balance - Normal   |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | XX | XX | XX |  1 |  0 |  0 |  1 || Off| On | On | Off| Off  | Balance - Reversed |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | XX | XX | XX |  1 |  0 |  1 |  0 || On | Off| Off| On | On   | Pan - Normal       |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+
// | XX | XX | XX |  1 |  0 |  1 |  1 || Off| On | On | Off| On   | Pan - Reversed     |
// +----+----+----+----+----+----+----++----+----+----+----+------+--------------------+

namespace Ham::IntegratedCircuitSystems::Shared
{

typedef Data_t ControlLeft_t;
namespace ControlLeft
{
    enum
    {
        Mute = 0x00,
        LL = 0x01,
        LR = 0x02,
        //--------------
        Reversed = 0x01,
        Stereo = 0x04,
        StereoReversed = Stereo | Reversed,
        //--------------
        Mono = 0x06,
        //--------------
        Balance = 0x08,
        BalanceReversed = Balance | Reversed,
        //--------------
        Pan = 0x0A,
        PanReversed = Pan | Reversed
    };
}

}