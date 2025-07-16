//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <ham/drivers/gravis/shared/regtype.h>

namespace Ham::Gravis::Shared::GF1
{

namespace Register
{
    enum
    {
        TimerData = 0x009
    };
}

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   |   |   |   |   +---- Timer 1 Start
//   |   |   |   |   |   |   +-------- Timer 2 Start
//   |   |   |   |   |   +------------ Reserved (Set to 0)
//   |   |   |   |   +---------------- Reserved (Set to 0)
//   |   |   |   +-------------------- Reserved (Set to 0)
//   |   |   +------------------------ Mask Timer 2
//   |   +---------------------------- Mask Timer 1
//   +-------------------------------- Reset Timer IRQ

// Bit 0   - Start up timer #1
// Bit 1   - Start up timer #2
// Bit 5   - Mask off timer 2
// Bit 6   - Mask off timer 1
// Bit 7   - Clear Timer IRQ

typedef uint8_t TimerData_t;
namespace TimerData
{

    enum
    {
        Timer1Start = 0x01,
        Timer2Start = 0x02,
        Timer1Mask = 0x20,
        Timer2Mask = 0x40,
        ClearTimerInterrupt = 0x80
    };

    namespace Shift
    {
        enum
        {
            Timer1Start = 0x00,
            Timer2Start = 0x01,
            Timer1Mask = 0x05,
            Timer2Mask = 0x06,
            ClearTimerInterrupt = 0x07
        };
    }

    inline TimerData_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::TimerData);
    }

    inline void Write(Register_t baseAddress, TimerData_t value)
    {
        SYS_WritePortByte(baseAddress + Register::TimerData, value);
    }

}

}
