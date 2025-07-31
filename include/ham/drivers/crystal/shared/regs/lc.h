//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        LoopbackControl = 0x0D
    };
}

// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | LBA5   LBA4   LBA3   LBA2   LBA1   LBA0 | res  | LBE  |
// +------+------+------+------+------+------+------+------+

// LBE         Loopback Enable: This bit will enable
//             the loopback mode of the CS4248 from
//             the ADC's output to the DACs. When
//             enabled, the data from the ADC's are
//             digitally mixed with other data being
//             delivered to the DACs.
//                 0 - Loopback disabled
//                 1 - Loopback enabled

// LBA5-LBAO   Loopback Attenuation: These bits
//             determine the attenuation of the
//             loopback from ADC to DAC. Each
//             attenuation step is -1.5 dB.
//             See Table 7.

// This register's initial state after reset is: 000000x0

// Table 7. Loopback Attenuation
//     LBA5 LBA4 LBA3 LBA2 LBA1 LBAO   Level
//  0    0    0    0    0    0    0      0.0 dB
//  1    0    0    0    0    0    1     -1.5 dB
//  2    0    0    0    0    0    0     -3.0 dB
//  3    0    0    0    0    0    1     -4.5 dB
//  .              .                      .
//  .              .                      .
//  .              .                      .
// 60    1    1    1    1    0    0    -90.0 dB
// 61    1    1    1    1    0    1    -91.5 dB
// 62    1    1    1    1    1    0    -93.0 dB
// 63    1    1    1    1    1    1    -94.5 dB

typedef uint8_t LoopbackControl_t;
namespace LoopbackControl
{

    enum
    {
        Control = 0x01,
        Disable = 0x00,
        Enable = 0x01,
        Attenuation = 0xFC
    };

    namespace Shift
    {
        enum
        {
            Control = 0x00,
            Attenuation = 0x02
        };
    };

}

}