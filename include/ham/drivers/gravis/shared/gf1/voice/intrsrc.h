//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/data.h>
#include <ham/drivers/gravis/shared/gf1/voice/vocbase.h>

namespace Ham::Gravis::Shared::GF1::Voice
{

namespace Register
{
    enum
    {
        InterruptSource = 0x0f
    };
}

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   +---+---+---+---+---- Interrupting voice #
//   |   |   +------------------------ 1
//   |   +---------------------------- Volume Ramp IRQ pending
//   +-------------------------------- WaveTable IRQ pending

// Bit 0-4     - Voice # (0-31) of interrupting voice
// Bit 5       - ALWAYS a 1
// Bit 6       - 0 = Volume Ramp IRQ occurred
// Bit 7       - 0 = Wavetable IRQ occurred

// Note: This is a global read only register. There is only 1 for ALL voices. You MUST service
// any indicated IRQ's since a read of this port will clear the associated IRQ bits in the particular
// voice's control and/or volume control registers.

// Note: It is possible that multiple voices could interrupt at virtually the same time. In this case,
// this register will behave like a fifo. When in your IRQ handler, keep reading (and servicing) this
// register until you do a read with both IRQ bits set to a 1. This means there are no voice IRQs left
// to deal with.

// Note: Since it is possible to get ANOTHER IRQ from the same voice for the SAME reason,
// you must ignore any subsequent IRQ from that voice while in the IRQ handler. For example,
// when a voice hits its end position and generates an IRQ back to your application, it will continue
// to generate IRQ's until either the voice is stopped, the IRQ enable is turned off, or the end
// location is moved.

typedef Indexed::Register8_t InterruptSource_t;
namespace InterruptSource
{

    enum
    {
        Voice = 0x1f,
        VolumeRamp = 0x40,
        WaveTable = 0x80
    };

    namespace Shift
    {
        enum
        {
            Voice = 0x00,
            VolumeRamp = 0x06,
            WaveTable = 0x07
        };
    }

    inline InterruptSource_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::InterruptSource) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, InterruptSource_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::InterruptSource) + uint8_t(Register::BaseWrite), value);
    }

}

}