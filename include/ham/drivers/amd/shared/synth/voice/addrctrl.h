//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/amd/shared/synth/data.h>
#include <ham/drivers/amd/shared/synth/voice/vocbase.h>

namespace Ham::Amd::Shared::Synthesizer::Voice
{

namespace Register
{
    enum
    {
        AddressControl = 0x00,
    };
}

// SACI—Synthesizer Address Control

// Address:    P3XR+5h read/write; index IGIDXR=00h write or IGIDXR=80h read; voice
//             index SVSR=(00h through 1Fh)

// Default: 01h

// This register controls how the synthesizer module addresses local memory and the local
// memory data width.

// +------------+------------+------------+---------------+------------+------------+------------+------------+
// | Bit 7      | Bit 6      | Bit 5      | Bit 4         | Bit 3      | Bit 2      | Bit 1      | Bit 0      |
// +------------+------------+------------+---------------+------------+------------+------------+------------+
// | Wavetable  | Direction* | Wavetable  | Bidirectional | Loop       | Data Width | Stop 1     | Stop 0     |
// | IRQ*       |            | IRQ Enable | Loop Enable   | Enable     |            |            |            |
// +------------+------------+------------+---------------+------------+------------+------------+------------+
// * Self-modifying bits

// Bit 7:  Wavetable IRQ. When High, the Wavetable IRQ Enable bit has been set
//         High and the synthesizer address has crossed a boundary set by the start
//         or end addresses. This bit is set Low when the voice's interrupt condition
//         has been loaded into the Synthesizer Voices IRQ register (SVII) and a value
//         of 8Fh has been written to the General Index register (IGIDXR). This bit can
//         also be set Low to clear an interrupt or set High to cause an interrupt.

// Bit 6:  Direction. This bit sets the direction in which the wavetable data in local
//         memory is addressed. If set Low, the address increases towards the
//         boundary set by the Address End registers. If set High, the address
//         decreases towards the boundary set by the Address Start registers. This bit
//         is modified by internal logic when the Bidirectional Loop Enable bit (SACI[4])
//         is set High.

// Bit 5:  Wavetable IRQ Enable. If set High, the Wavetable IRQ bit (SACI[7]) is set
//         High when an address boundary is crossed. If set Low, SACI[7] is cleared
//         and cannot be set.

// Bit 4:  Bidirectional Loop Enable. If set High, the synthesizer current address
//         changes direction at both the start and the end address boundaries. If set
//         Low, the current address continues to loop in the same direction when end
//         points are crossed. This bit has no effect if the Loop Enable bit (SACI[3]) is
//         set Low.

// Bit 3:  Loop Enable. If set High, the address loops between address boundaries
//         controlled by the Bidirectional Loop Enable (SACI[4]) and Direction
//         (SACI[6]) bits. If set Low, the address moves to the boundary indicated by
//         the start or end addresses or beyond if the Enable PCM Operation bit in the
//         Synthesizer Volume Control register (SVCI[2]) is set High.

// Bit 2:  Data Width. This bit determines whether local memory is addressed as
//         16-bit or 8-bit data. If set High, local memory is addressed as 16-bit data. If
//         set Low, local memory is addressed as 8-bit data.

// Bit 1:  Stop 1. If set High, stops voice activity. Both this bit and the Stop 0 bit
//         (SACI[0]) must be Low for a voice to operate.

// Bit 0:  Stop 0. This bit is modified by the address control logic. If a voice is set to
//         stop at a boundary, this bit is set High when the boundary is crossed. Set it
//         High to stop a voice. When read, it shows the status of a voice. Both the
//         Stop 1 bit (SACI[1]) and this bit must be Low for a voice to operate.

typedef Indexed::Register8_t AddressControl_t;
namespace AddressControl
{

    enum
    {
        Status = 0x01,
        Playing = 0x00,
        Stopped = 0x01,
        Control = 0x02,
        Play = 0x00,
        Stop = 0x02,
        BitWidth = 0x04,
        Bit8 = 0x00,
        Bit16 = 0x04,
        LoopControl = 0x08,
        LoopDisable = 0x00,
        LoopEnable = 0x08,
        LoopType = 0x10,
        LoopForward = 0x00,
        LoopBidirectional = 0x10,
        InterruptControl = 0x20,
        InterruptDisable = 0x00,
        InterruptEnable = 0x20,
        Direction = 0x40,
        Forward = 0x00,
        Backward = 0x40,
        InterruptStatus = 0x80,
        InterruptClear = 0x00,
        InterruptPending = 0x80
    };

    namespace Shift
    {
        enum
        {
            StopStatus = 0x00,
            StopControl = 0x01,
            BitWidth = 0x02,
            LoopControl = 0x03,
            LoopType = 0x04,
            InterruptControl = 0x05,
            Direction = 0x06,
            InterruptStatus = 0x07
        };
    }

    inline AddressControl_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::AddressControl) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, AddressControl_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::AddressControl) + uint8_t(Voice::Register::BaseWrite), value);
    }

}

}