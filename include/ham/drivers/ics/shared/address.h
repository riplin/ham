//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <ham/drivers/ics/regtype.h>

namespace Ham::IntegratedCircuitSystems::Shared
{

// The address register is used to point to internal registers (at-
// tenuator and control).

// Data Definition
// +------+------+------+------+------+------+------+
// |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
// |  XX  |  --  |  --  |  --  |  --  |  --  |  --  |
// +------+------+------+------+------+------+------+
//        |____________________|____________________|
//         Attenuator Selection  Control Selection

// Attenuator and Control Register Selection
// +-----------------------------------------------------+
// |                                    Attenuator and   |
// |                                    Control Register |
// +----+----+----+----+----+----+----+------------------+
// | D6 | D5 | D4 | D3 | D2 | D1 | D0 | Definition       |
// +----+----+----+----+----+----+----+------------------+
// | XX | XX | XX | XX |  0 |  0 |  0 | Control Left     |
// +----+----+----+----+----+----+----+------------------+
// | XX | XX | XX | XX |  0 |  0 |  1 | Control Right    |
// +----+----+----+----+----+----+----+------------------+
// | XX | XX | XX | XX |  0 |  1 |  0 | Attenuator Left  |
// +----+----+----+----+----+----+----+------------------+
// | XX | XX | XX | XX |  0 |  1 |  1 | Attenuator Right |
// +----+----+----+----+----+----+----+------------------+
// | XX | XX | XX | XX |  1 |  0 |  X | Pan/Balance      |
// +----+----+----+----+----+----+----+------------------+
// | XX |  0 |  0 |  0 | XX | XX | XX | Pair 1           |
// +----+----+----+----+----+----+----+------------------+
// | XX |  0 |  0 |  1 | XX | XX | XX | Pair 2           |
// +----+----+----+----+----+----+----+------------------+
// | XX |  0 |  1 |  0 | XX | XX | XX | Pair 3           |
// +----+----+----+----+----+----+----+------------------+
// | XX |  0 |  1 |  1 | XX | XX | XX | Pair 4           |
// +----+----+----+----+----+----+----+------------------+
// | XX |  1 |  0 |  0 | XX | XX | XX | Pair 5           |
// +----+----+----+----+----+----+----+------------------+
// | XX |  1 |  0 |  1 | XX | XX | XX | Master           |
// +----+----+----+----+----+----+----+------------------+

typedef uint8_t Address_t;
namespace Address
{
    enum
    {
        Control = 0x00,
        Attenuator = 0x02,
        Left = 0x00,
        Right = 0x01,
        ControlLeft = Control | Left,
        ControlRight = Control | Right,
        AttenuatorLeft = Attenuator | Left,
        AttenuatorRight = Attenuator | Right,
        BalancePan = 0x04,

        Channel = 0x38,
        Pair1 = 0x00,
        Pair2 = 0x08,
        Pair3 = 0x10,
        Pair4 = 0x18,
        Pair5 = 0x20,
        Master = 0x28
    };

    namespace Shift
    {
        enum
        {
            Control = 0x00,
            Attenuator = 0x00,
            Channel = 0x03
        };
    }

    inline void Write(Register_t addressPort, Address_t channel, Address_t reg)
    {
        SYS_WritePortByte(addressPort, channel | reg);
    }

}
}