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
        Status = 0x100
    };
}

// Here are the bit definitions for the MIDI status byte. It is located at address 3X0 hex and is read-only.

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   |   |   |   |   +---- Receive reg. full
//   |   |   |   |   |   |   +-------- Transmit reg. empty
//   |   |   |   |   |   +------------ Reserved
//   |   |   |   |   +---------------- Reserved
//   |   |   |   +-------------------- Framing Error
//   |   |   +------------------------ Overrun error
//   |   +---------------------------- Reserved
//   +-------------------------------- Interrupt pending

// The MIDI control interface behaves identically to a 6850 UART.

typedef uint8_t Status_t;
namespace Status
{

    enum
    {
        Receive = 0x01,
        ReceiveEmpty = 0x00,
        ReceiveFull = 0x01,

        Transmit = 0x02,
        TransmitFull = 0x00,
        TransmitEmpty = 0x02,

        Framing = 0x10,
        FramingClear = 0x00,
        FramingError = 0x10,

        Overrun = 0x20,
        OverrunClear = 0x00,
        OverrunError = 0x20,

        Interrupt = 0x80,
        InterruptClear = 0x00,
        InterruptPending = 0x80
    };

    namespace Shift
    {
        enum
        {
            Receive = 0x00,
            Transmit = 0x01,
            Framing = 0x04,
            Overrun = 0x05,
            Interrupt = 0x07
        };
    }

    inline Status_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::Status);
    }

}

}
