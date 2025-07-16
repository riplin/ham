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
        InterruptStatus = 0x006
    };
}

// CAUTION: Note that this is at 2X6 *** NOT 3X6 ***.

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   |   |   |   |   +---- MIDI Transmit IRQ
//   |   |   |   |   |   |   +-------- MIDI Receive IRQ
//   |   |   |   |   |   +------------ Timer 1 IRQ
//   |   |   |   |   +---------------- Timer 2 IRQ
//   |   |   |   +-------------------- Reserved (Set to 0)
//   |   |   +------------------------ WaveTable IRQ (any voice)
//   |   +---------------------------- Volume Ramp IRQ (any voice)
//   +-------------------------------- DMA TC IRQ (DRAM or Sample)

typedef uint8_t InterruptStatus_t;
namespace InterruptStatus
{

    enum
    {
        MidiTransmit = 0x01,
        MidiReceive = 0x02,
        Timer1 = 0x04,
        Timer2 = 0x08,
        WaveTable = 0x20,
        VolumeRamp = 0x40,
        DmaTC = 0x80
    };

    namespace Shift
    {
        enum
        {
            MidiTransmit = 0x00,
            MidiReceive = 0x01,
            Timer1 = 0x02,
            Timer2 = 0x03,
            WaveTable = 0x05,
            VolumeRamp = 0x06,
            DmaTC = 0x07
        };
    }

    inline InterruptStatus_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::InterruptStatus);
    }

}

}
