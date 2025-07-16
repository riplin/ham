//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/data.h>

namespace Ham::Gravis::Shared::GF1::Global
{

namespace Register
{
    enum
    {
        TimerCount1 = 0x46,
        TimerCount2 = 0x47,
    };
}

// These counts are loaded by the application and then they will count up to $FF and generate and IRQ. Timer 1 has a
// granularity of 80 microseconds (0.00008 sec) and Timer 2 has a granularity of 320 microseconds (0.00032 sec).

typedef Indexed::Register8_t TimerCount_t;
namespace TimerCount
{

    enum
    {
        Value = 0xff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    inline void Write1(Register_t baseAddress, TimerCount_t value)
    {
        Data::Write8High(baseAddress, Register::TimerCount1, value);
    }

    inline void Write2(Register_t baseAddress, TimerCount_t value)
    {
        Data::Write8High(baseAddress, Register::TimerCount2, value);
    }

}

}
