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
        DmaControl = 0x00b
    };
}

// DMA control register I/O W 2XB
// (2X0- bit 6 = 0)

// Bits 2-0    DMA Select Register 1
//                 0 = NO DMA
//                 1 = DMA1
//                 2 = DMA3
//                 3 = DMA5
//                 4 = DMA6
//                 5 = DMA7
// Bits 5-3    DMA Select Register 2
//                 0 = NO DMA
//                 1 = DMA1
//                 2 = DMA3
//                 3 = DMA5
//                 4 = DMA6
//                 5 = DMA7
// Bit 6       - Combine Both on the same DMA channel.
// Bit 7       - Reserved (Set to 0).

// Note: If the channels are sharing an DMA, channel 2's DMA must be set to 0 and turn on bit 6.
// A bus conflict will occur if both latches are programmed with the same DMA #.

// C programmers can refer to the UltraSetInterface routine in INIT.C of the lowlevel source code for the proper
// sequence for programming these latches. If the order is not right, unpredictable things may happen.
// Changing the IRQ settings will usually cause an IRQ on the OLD IRQ because it is no longer being driven low by
// the latches and it will tend to float up. That low to high transition causes an IRQ on the PC. Normally, this is not a
// problem, but it is something to be aware of.

typedef uint8_t DmaControl_t;
namespace DmaControl
{

    enum
    {
        Dma1Selector = 0x07,
        Dma1SelectNone = 0x00,
        Dma1Select1 = 0x01,
        Dma1Select3 = 0x02,
        Dma1Select5 = 0x03,
        Dma1Select6 = 0x04,
        Dma1Select7 = 0x05,

        Dma2Selector = 0x38,
        Dma2SelectNone = 0x00,
        Dma2Select1 = 0x08,
        Dma2Select3 = 0x10,
        Dma2Select5 = 0x18,
        Dma2Select6 = 0x20,
        Dma2Select7 = 0x28,

        CombineDma = 0x40
    };

    namespace Shift
    {
        enum
        {
            Dma1Selector = 0x00,
            Dma2Selector = 0x03,
            CombineDma = 0x06
        };
    }

    inline void Write(Register_t baseAddress, DmaControl_t value)
    {
        SYS_WritePortByte(baseAddress + Register::DmaControl, value);
    }

}

}
