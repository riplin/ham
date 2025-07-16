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
        VoiceControl = 0x00
    };
}

// Changing the start and end points of a voice while its playing can have some strange side effects. For example, if you
// change end position to a lower location than it is currently playing, you will get an IRQ (if they are enabled). Also,
// since the high and low bytes are set individually and asynchronously to when the GF1 is working on a voice, it is
// possible to get an unexpected IRQ if the current position and the new end position cross.

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   |   |   |   |   +---- Voice Stopped
//   |   |   |   |   |   |   +-------- Stop Voice
//   |   |   |   |   |   +------------ 16 bit data
//   |   |   |   |   +---------------- Loop enable
//   |   |   |   +-------------------- Bi-directional loop enable
//   |   |   +------------------------ Wave table IRQ
//   |   +---------------------------- Direction of movement
//   +-------------------------------- IRQ pending

// * Bit 0     - 1 = Voice is stopped. This gets set by hitting the end address (not looping) or by
//                   setting bit 1 in this reg.
//   Bit 1     - 1 = Stop Voice. Manually force voice to stop.
//   Bit 2     - 1 = 16 bit wave data, 0 = 8 bit data
//   Bit 3     - 1 = Loop to begin address when it hits the end address.
//   Bit 4     - 1 = Bi-directional looping enabled
//   Bit 5     - 1 = Enable wavetable IRQ. Generate an IRQ when the voice hits the end address. Will
//                   generate IRQ even if looping is enabled.
// * Bit 6     - 1 = Decreasing addresses, 0 = increasing addresses. It is self-modifying because it might shift
//                   directions when it hits one of the loop boundaries and looping is enabled.
// * Bit 7     - 1 = WaveTable IRQ pending. If IRQ's are enabled and looping is NOT enabled, an IRQ will be
//                   constantly generated until the voice is stopped. This means that you may get more than 1 IRQ
//                   if it isn't handled properly.

typedef Indexed::Register8_t VoiceControl_t;
namespace VoiceControl
{

    enum
    {
        Status = 0x01,  // *
        Playing = 0x00, // *
        Stopped = 0x01, // *

        Control = 0x02,
        Play = 0x00,//?
        Stop = 0x02,

        BitWidth = 0x04,
        Bits8 = 0x00,
        Bits16 = 0x04,

        LoopToBegin = 0x08,

        BidirectionalLooping = 0x10,

        InterruptControl = 0x20,
        InterruptDisable = 0x00,
        InterruptEnable = 0x20,

        Direction = 0x40,   // *
        Forward = 0x00,     // *
        Backward = 0x40,    // *

        InterruptState = 0x80,  // *
        InterruptClear = 0x00,  // *
        InterruptPending = 0x80 // *
    };

    namespace Shift
    {
        enum
        {
            Status = 0x00,
            Control = 0x01,
            BitWidth = 0x02,
            LoopToBegin = 0x03,
            BidirectionalLooping = 0x04,
            InterruptControl = 0x05,
            Direction = 0x06,
            InterruptState = 0x07
        };
    }

    inline VoiceControl_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::VoiceControl) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, VoiceControl_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::VoiceControl) + uint8_t(Register::BaseWrite), value);
    }

}

}