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
        VolumeControl = 0x0D,
    };
}

// SVCI—Synthesizer Volume Control

// Address:    P3XR+5h read/write; index IGIDXR=0Dh write or IGIDXR=8Dh read; voice
//             index SVSR=(00h through 1Fh)

// Default: 01h

// This register controls how the looping component of a voice's volume multiplier moves from
// volume start to volume end. This register also contains the Enable PCM Operation bit that
// controls local memory addressing to allow a voice to continuously play blocks of PCM data.

// +-------------+------------+------------+---------------+------------+------------+------------+------------+
// | Bit 7       | Bit 6      | Bit 5      | Bit 4         | Bit 3      | Bit 2      | Bit 1      | Bit 0      |
// +-------------+------------+------------+---------------+------------+------------+------------+------------+
// | Volume IRQ* | Direction* | Volume IRQ | Bidirectional | Loop       | Enable PCM | Stop 1     | Stop 0*    |
// |             |            | Enable     | Loop Enable   | Enable     | Operation  |            |            |
// +-------------+------------+------------+---------------+------------+------------+------------+------------+
// * Self-modifying bits

// Bit 7:  Volume IRQ. When High, the Volume IRQ Enable bit (SVCI[5]) has been
//         set and the volume has crossed a boundary point set by the start or the end
//         volume. This bit is cleared when the voice's interrupt condition has been
//         loaded into the Synthesizer Voices IRQ register (SVII) and a value of 8Fh
//         has been written to the General Index register (IGIDXR). This bit can also
//         be set Low to clear an interrupt, or set High to cause an interrupt.

// Bit 6:  Direction. If set High, volume decreases. If set Low, volume increases. This
//         bit is modified by internal logic when the Bidirectional Loop Enable bit
//         (SVCI[4]) is set High.

// Bit 5:  Volume IRQ Enable. If set High, the Volume IRQ bit (SVCI[7]) is set when
//         a volume boundary is crossed. If set Low, SVCI[7] is cleared and cannot be
//         set.

// Bit 4:  Bidirectional Loop Enable. If set High, the volume changes directions at
//         both the start and end volumes. If set Low, the volume continues to loop in
//         the same direction when end points are crossed. This bit has no effect if the
//         Loop Enable bit (SVCI[3]) is set Low.

// Bit 3:  Loop Enable. If set High, the volume loops between end points controlled
//         by the Bidirectional Loop Enable (SVCI[4]) and Direction (SVCI[6]) bits. If
//         set Low, the volume moves to a volume boundary and then the volume is
//         held constant.

// Bit 2:  Enable PCM Operation. If set High, the address continues past a wavetable
//         address boundary, which allows for continuous play of PCM data. For
//         information about how this bit affects the interpolation process, see “Address
//         Control” on page 7-9.

// Bit 1:  Stop 1. If set High, stops the change in the looping component of volume.
//         Both this bit and the Stop 0 (SVCI[0]) bit must be set Low to allow the looping
//         component of volume to change.

// Bit 0:  Stop 0. This bit is modified by the volume looping logic. If volume is set to
//         stop at a boundary, this bit is set High when the boundary is crossed. It can
//         also be set High to stop volume looping. When read, it shows the status of
//         volume looping. Both the Stop 1 bit (SVCI[1]) and this bit must be set Low
//         to allow the looping component of volume to change.

typedef Indexed::Register8_t VolumeControl_t;
namespace VolumeControl
{

    enum
    {
        Status = 0x01,
        Playing = 0x00,
        Stopped = 0x01,
        Control = 0x02,
        Play = 0x00,
        Stop = 0x02,
        PcmControl = 0x04,
        PcmBounded = 0x00,
        PcmUnbounded = 0x04,
        LoopControl = 0x08,
        LoopDisable = 0x00,
        LoopEnable = 0x08,
        Directionality = 0x10,
        Forward = 0x00,
        Bidirectional = 0x10,
        InterruptControl = 0x20,
        InterruptDisable = 0x00,
        InterruptEnable = 0x20,
        Direction = 0x40,
        Up = 0x00,
        Down = 0x40,
        InterruptStatus = 0x80,
        InterruptClear = 0x00,
        InterruptPending = 0x80
    };

    namespace Shift
    {
        enum
        {
            Status = 0x00,
            Control = 0x01,
            PcmControl = 0x02,
            LoopControl = 0x03,
            Directionality = 0x04,
            InterruptControl = 0x05,
            Direction = 0x06,
            InterruptStatus = 0x07
        };
    }

    inline VolumeControl_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::VolumeControl) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, VolumeControl_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::VolumeControl) + uint8_t(Voice::Register::BaseWrite), value);
    }

}

}