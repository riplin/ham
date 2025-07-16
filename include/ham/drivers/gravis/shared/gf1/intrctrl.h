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
        InterruptControl = 0x00b
    };
}

// IRQ control register I/O W 2XB
// (2X0- bit 6 = 1)

// Bits 2-0    Channel 1 (GF1) IRQ Selector
//                 0 = RESERVED, DO NOT USE
//                 1 = IRQ2
//                 2 = IRQ5
//                 3 = IRQ3
//                 4 = IRQ7
//                 5 = IRQ11
//                 6 = IRQ12
//                 7 = IRQ15
// Bits 5-3    Channel 2 (MIDI) IRQ selector
//                 0 = No Interrupt
//                 1 = IRQ2
//                 2 = IRQ5
//                 3 = IRQ3
//                 4 = IRQ7
//                 5 = IRQ11
//                 6 = IRQ12
//                 7 = IRQ15
// Bit 6       1 = Combine Both IRQS using Channel 1's IRQ
// Bit 7       Reserved (Set to 0)

// Note: If the channels are sharing an IRQ, channel 2's IRQ must be set to 0 and turn on bit 6. A
// bus conflict will occur if both latches are programmed with the same IRQ #.

typedef uint8_t InterruptControl_t;
namespace InterruptControl
{

    enum
    {
        Gf1InterruptSelector = 0x07,
        Gf1Interrupt2 = 0x01,
        Gf1Interrupt5 = 0x02,
        Gf1Interrupt3 = 0x03,
        Gf1Interrupt7 = 0x04,
        Gf1Interrupt11 = 0x05,
        Gf1Interrupt12 = 0x06,
        Gf1Interrupt15 = 0x07,

        MidiInterruptSelector = 0x38,
        MidiInterruptNone = 0x00,
        MidiInterrupt2 = 0x08,
        MidiInterrupt5 = 0x10,
        MidiInterrupt3 = 0x18,
        MidiInterrupt7 = 0x20,
        MidiInterrupt11 = 0x28,
        MidiInterrupt12 = 0x30,
        MidiInterrupt15 = 0x38,

        CombineInterrupts = 0x40
    };

    namespace Shift
    {
        enum
        {
            Gf1InterruptSelector = 0x00,
            MidiInterruptSelector = 0x03,
            CombineInterrupts = 0x06
        };
    }

    inline void Write(Register_t baseAddress, InterruptControl_t value)
    {
        SYS_WritePortByte(baseAddress + Register::InterruptControl, value);
    }

}

}
