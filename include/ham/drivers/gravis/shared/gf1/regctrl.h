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
        RegisterControl = 0x00f
    };
}

// This register is only valid for UltraSound boards that are at or above revision 3.4. It is not valid for boards which
// have a prior revision number.

// On 3.4 and above boards, there is a bank of 6 registers that exist at location 2XB. Register 2XF is used to select
// which one is being talked to.

// Register # Use
// 0 Same as pre-3.4 boards
// 1-4 Unused - Reserved
// 5 Write a 0 to clear IRQs on power-up
// 6 'Jumper register'

// Jumper register definition:

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   |   |   |   |   +---- Reserved (Set to 0)
//   |   |   |   |   |   |   +-------- 1=Enable MIDI port addr decode
//   |   |   |   |   |   +------------ 1=Enable joystick port decode
//   |   |   |   |   +---------------- Reserved (Set to 0)
//   |   |   |   +-------------------- Reserved (Set to 0)
//   |   |   +------------------------ Reserved (Set to 0)
//   |   +---------------------------- Reserved (Set to 0)
//   +-------------------------------- Reserved (Set to 0)

typedef uint8_t RegisterControl_t;
namespace RegisterControl
{
    enum
    {
        Default = 0x00,
        ClearInterruptPort = 0x05,
        JumperPort = 0x06
    };

    inline RegisterControl_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::RegisterControl);
    }

    inline void Write(Register_t baseAddress, RegisterControl_t value)
    {
        SYS_WritePortByte(baseAddress + Register::RegisterControl, value);
    }

}

}
