//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/data.h>

namespace Ham::Gravis::Shared::GF1::Global
{

namespace Register
{
    enum
    {
        SamplingControl = 0x49
    };
}

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   |   |   |   |   +---- Start sampling
//   |   |   |   |   |   |   +-------- Mode (0=mono, 1=stereo)
//   |   |   |   |   |   +------------ DMA width (0=8bit,1=16bit)
//   |   |   |   |   +---------------- Reserved (Set to 0)
//   |   |   |   +-------------------- Reserved (Set to 0)
//   |   |   +------------------------ DMA IRQ enable
//   |   +---------------------------- (Read) DMA IRQ pending
//   +-------------------------------- Invert MSB

// Bit 0 - If PC DMA controller is programmed, it will begin sampling as soon as this is enabled(1).
// Bit 1 - 0 = mono
//         1 = stereo
//         In stereo mode, the order of the data bytes is left is first, and right is second. 
//         If a 16 bit data channel is used, the left is in the lower byte.
// Bit 2 - DMA Channel width (0 = 8 bit, 1 = 16 bit)
// Bit 5 - Enable DMA terminal count IRQ
// Bit 6 - DMA terminal count IRQ pending
// Bit 7 - Flip bit 7 to get non-twos compliment data

typedef Indexed::Register8_t SamplingControl_t;
namespace SamplingControl
{

    enum
    {
        Status = 0x01,
        Stop = 0x00,
        Start = 0x01,

        Mode = 0x02,
        Mono = 0x00,
        Stereo = 0x02,

        DmaWidth = 0x04,
        Width8 = 0x00,
        Width16 = 0x04,

        DmaInterruptControl = 0x20,
        DmaInterruptDisable = 0x00,
        DmaInterruptEnable = 0x20,

        //Read:
        DmaInterruptState = 0x40,
        DmaInterruptClear = 0x00,
        DmaInterruptPending = 0x40,

        InvertTopBit = 0x80

    };

    namespace Shift
    {
        enum
        {
            Status = 0x00,
            Mode = 0x01,
            DmaWidth = 0x02,
            DmaInterruptControl = 0x05,
            DmaInterruptState = 0x06,
            InvertTopBit = 0x07
        };
    }

    inline SamplingControl_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, Register::SamplingControl);
    }

    inline void Write(Register_t baseAddress, SamplingControl_t value)
    {
        Data::Write8High(baseAddress, Register::SamplingControl, value);
    }

}

}
