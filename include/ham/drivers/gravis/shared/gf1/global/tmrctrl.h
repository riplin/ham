//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/data.h>

namespace Ham::Gravis::Shared::GF1::Global
{

namespace Register
{
    enum
    {
        TimerControl = 0x45
    };
}

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   |   |   |   |   +---- Reserved (Set to 0)
//   |   |   |   |   |   |   +-------- Reserved (Set to 0)
//   |   |   |   |   |   +------------ Enable Timer 1 IRQ
//   |   |   |   |   +---------------- Enable Timer 2 IRQ
//   |   |   |   +-------------------- Reserved (Set to 0)
//   |   |   +------------------------ Reserved (Set to 0)
//   |   +---------------------------- Reserved (Set to 0)
//   +-------------------------------- Reserved (Set to 0)

typedef Indexed::Register8_t TimerControl_t;
namespace TimerControl
{

    enum
    {
        Timer1 = 0x04,
        Timer1Disable = 0x00,
        Timer1Enable = 0x04,

        Timer2 = 0x08,
        Timer2Disable = 0x00,
        Timer2Enable = 0x08
    };

    namespace Shift
    {
        enum
        {
            Timer1 = 0x02,
            Timer2 = 0x03
        };
    }

    inline TimerControl_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, Register::TimerControl);
    }

    inline void Write(Register_t baseAddress, TimerControl_t value)
    {
        Data::Write8High(baseAddress, Register::TimerControl, value);
    }

}

}
