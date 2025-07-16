//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/data.h>

namespace Ham::Gravis::Shared::GF1::Global
{

namespace Register
{
    enum
    {
        Reset = 0x4c
    };
}

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   |   |   |   |   +---- Master Reset
//   |   |   |   |   |   |   +-------- DAC Enable
//   |   |   |   |   |   +------------ GF1 Master IRQ Enable
//   |   |   |   |   +---------------- Reserved (Set to 0)
//   |   |   |   +-------------------- Reserved (Set to 0)
//   |   |   +------------------------ Reserved (Set to 0)
//   |   +---------------------------- Reserved (Set to 0)
//   +-------------------------------- Reserved (Set to 0)

// Bit 0 - GF1 Master Reset. 0 = reset, 1 = run. As long as this is a 0, it will be held in a reset state.
// Bit 1 - Enable DAC output. DAC's will not run unless this bit is set.
// Bit 2 - Master IRQ enable. This bit MUST be set to get ANY of the GF1-generated IRQs (wavetable, volume, etc).

// This register will normally contain the value 0x07 while your application is running.

typedef Indexed::Register8_t Reset_t;
namespace Reset
{

    enum
    {
        Master = 0x01,
        MasterDisable = 0x00,
        MasterEnable = 0x01,

        Dac = 0x02,
        DacDisable = 0x00,
        DacEnable = 0x02,

        Interrupt = 0x04,
        InterruptDisable = 0x00,
        InterruptEnable = 0x04
    };

    namespace Shift
    {
        enum
        {
            Master = 0x00,
            Dac = 0x01,
            Interrupt = 0x02
        };
    }

    inline Reset_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, Register::Reset);
    }

    inline void Write(Register_t baseAddress, Reset_t value)
    {
        Data::Write8High(baseAddress, Register::Reset, value);
    }

}

}
