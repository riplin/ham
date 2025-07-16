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
        MixControl = 0x000
    };
}

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   |   |   |   |   +---- 0=Enable Line IN
//   |   |   |   |   |   |   +-------- 0=Enable Line OUT
//   |   |   |   |   |   +------------ 1=Enable MIC IN
//   |   |   |   |   +---------------- Enable latches
//   |   |   |   +-------------------- Combine Chan1 IRQ with Chan2
//   |   |   +------------------------ Enable MIDI loopback TxD to RxD
//   |   +---------------------------- Control Reg Select
//   +-------------------------------- Reserved (Set to 0)

// Bit 0   - Enable UltraSound line Input
// Bit 1   - Enable UltraSound Line output
// Bit 2   - Enable Stereo Mic Input
// Bit 3   - Enable latches. This provides power to the DMA/IRQ latches. Once these are enabled,
//           NEVER disable them. Disabling them will cause random IRQ's in the PC since the DMA and
//           IRQ lines are not being driven any more.
// Bit 4   - Combine Channel 1 (GF1) IRQ's with Channel 2 (MIDI)
// Bit 5   - Enable MIDI loopback. Any data sent out Transmit port will be looped back into the
//           input port.
// Bit 6   - Control Register Select. When this is set to a 1, the next IO write to 2XB will be to the
//           IRQ control latches. When this is set to a 0, the next IO write to 2XB will be to the DMA
//           channel latches. The write to 2XB for either of these MUST occur as the NEXT IOW or else the
//           write to 2XB will be locked out and not occur. This is to prevent an application that is probing
//           for cards to accidentally corrupt the latches.

typedef uint8_t MixControl_t;
namespace MixControl
{

    enum
    {
        LineInput = 0x01,
        LineInputDisable = 0x00,
        LineInputEnable = 0x01,

        LineOutput = 0x02,
        LineOuputDisable = 0x00,
        LineOutputEnable = 0x02,

        MicrophoneInput = 0x04,
        MicrophoneInputDisable = 0x00,
        MicrophoneInputEnable = 0x04,

        DmaInterruptLatches = 0x08,
        LatchesEnable = 0x08,

        CombineInterrupts = 0x10,
        InterruptsSeperate = 0x00,
        InterruptsCombined = 0x10,

        MidiLoopback = 0x20,
        LoopbackDisable = 0x00,
        LoopbackEnable = 0x20,

        ControlSelect = 0x40,
        SelectDma = 0x00,
        SelectInterrupt = 0x40
    };

    namespace Shift
    {
        enum
        {
            LineInput = 0x00,
            LineOutput = 0x01,
            MicrophoneInput = 0x02,
            DmaInterruptLatches = 0x03,
            CombineInterrupts = 0x04,
            MidiLoopback = 0x05,
            ControlSelect = 0x06
        };
    }

    inline void Write(Register_t baseAddress, MixControl_t value)
    {
        SYS_WritePortByte(baseAddress + Register::MixControl, value);
    }

}

}
