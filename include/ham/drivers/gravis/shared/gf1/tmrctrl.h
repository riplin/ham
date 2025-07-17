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
        TimerControl = 0x008
    };
}

// This register maps to the same location as the ADLIB board's control register. Writing a 4 here selects the timer
// stuff. Bit 6 will be set if timer #1 has expired. Bit 5 will be set it timer #2 has expired.

typedef uint8_t TimerControl_t;
namespace TimerControl
{
    enum
    {
        //Write:
        Timer1DataSelect = 0x02,
        Timer2DataSelect = 0x03,
        TimerControlSelect = 0x04,

        //Read:
        Timer2Expired = 0x20,
        Timer1Expired = 0x40,
        TimersExpired = 0x80
    };

    inline TimerControl_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::TimerControl);
    }

    inline void Write(Register_t baseAddress, TimerControl_t value)
    {
        SYS_WritePortByte(baseAddress + Register::TimerControl, value);
    }

}

}
