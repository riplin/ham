//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <ham/drivers/gravis/shared/regtype.h>

namespace Ham::Gravis::Shared::Midi
{

namespace Register
{
    enum
    {
        Control = 0x100
    };
}

// Here are the bit definitions for the MIDI control byte. It is located at address 3X0 hex and is write only.

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   |   |   |   +---+---- 1 = Master reset (when set)
//   |   |   |   |   |   +------------ Reserved
//   |   |   |   |   +---------------- Reserved
//   |   |   |   +-------------------- Reserved
//   |   +---+------------------------ 0,1 = xmit IRQ enabled
//   +-------------------------------- 1 = Receive IRQ enabled

// Bit 0 & 1 will cause a master reset when toggled high and then low. They must be left low when using port. This will
// normally cause a transmit buffer empty IRQ.

typedef uint8_t Control_t;
namespace Control
{

    enum
    {
        State = 0x03,
        Enable = 0x00,
        Reset = 0x03,

        TransmitInterrupt = 0x60,
        TransmitInterruptDisable = 0x00,
        TransmitInterruptEnable = 0x40,

        ReceiveInterrupt = 0x80,
        ReceiveInterruptDisable = 0x00,
        ReceuveInterruptEnable = 0x80
    };

    namespace Shift
    {
        enum
        {
            State = 0x00,
            TransmitInterrupt = 0x05,
            ReceiveInterrupt = 0x07,
        };
    }

    inline void Write(Register_t baseAddress, Control_t value)
    {
        SYS_WritePortByte(baseAddress + Register::Control, value);
    }

}

}
