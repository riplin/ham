//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/data.h>

namespace Ham::Gravis::Shared::GF1::Global
{

namespace Register
{
    enum
    {
        DramDmaControl = 0x41
    };
}

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   |   |   |   |   +---- Enable DMA (1=go)
//   |   |   |   |   |   |   +-------- DMA direction (1=read)
//   |   |   |   |   |   +------------ DMA channel width
//   |   |   |   |   |
//   |   |   |   +---+---------------- DMA Rate divider
//   |   |   +------------------------ DMA IRQ Enable
//   |   +---------------------------- (R) DMA IRQ Pending
//   |                                 (W) DATA SIZE (0=8bit,1=16bit)
//   +-------------------------------- Invert MSB (write only)

// Bit 0       - Enable the DMA channel. The GF1 will begin sending DMA ACK protocol. If PC DMA controller is
//               programmed, data will begin being transferred. If not, data will move as soon as it is programmed.

// Bit 1       - DMA transfer direction. Read(1) is taking data OUT of the UltraSound, Write(0) sends data to it.

// Bit 2       - 0 = if DMA channel is an 8 bit channel (0-3).
//               1 = If it is a 16 bit channel (4-7)
//               Note: This is INDEPENDENT of the data size.

// Bit 3,4     - DMA Rate divisor. The Maximum rate is approx. 650 khz.
//               0,0 = divide by 1
//               0,1 = divide by 2
//               1,0 = divide by 3
//               1,1 = divide by 4

// Bit 5       - DMA terminal count interrupt enable

// Bit 6       - Read  - DMA terminal count IRQ pending
//               Write - Data size 0 = 8 Bit data
//                                 1 = 16 bit data
//               Note: Data size is independent of channel size

// Bit 7       - 1 = Invert High bit to flip data to twos complement form.
//               Note: This flips bit 7 for 8 bit data and bit 15 for 16 bit data.

typedef Indexed::Register8_t DramDmaControl_t;
namespace DramDmaControl
{

    enum
    {
        State = 0x01,
        Disable = 0x00,
        Enable = 0x01,

        DataDirection = 0x02,
        DataRead = 0x02,
        DataWrite = 0x00,

        ChannelWidth = 0x04,
        Width8 = 0x00,
        Width16 = 0x04,

        DmaRateDivisor = 0x18,
        Divide1 = 0x00,
        Divide2 = 0x08,
        Divide3 = 0x10,
        Divide4 = 0x18,

        DmaInterruptControl = 0x20,
        DmaInterruptDisable = 0x00,
        DmaInterruptEnable = 0x20,

        //Read:
        DmaInterruptState = 0x40,
        DmaInterruptClear = 0x00,
        DmaInterruptPending = 0x40,

        //Write:
        DataSize = 0x40,
        Data8 = 0x00,
        Data16 = 0x40,

        InvertTopBit = 0x80
    };

    namespace Shift
    {
        enum
        {
            State = 0x00,
            Direction = 0x01,
            ChannelWidth = 0x02,
            DmaRateDivisor = 0x03,
            DmaInterruptControl = 0x05,
            DmaInterruptPending = 0x06,
            DataSize = 0x06,
            InvertTopBit = 0x07
        };
    }

    inline DramDmaControl_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, Register::DramDmaControl);
    }

    inline void Write(Register_t baseAddress, DramDmaControl_t value)
    {
        Data::Write8High(baseAddress, Register::DramDmaControl, value);
    }

}

}
