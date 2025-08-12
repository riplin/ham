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
        VoiceInterrupt = 0x0f,
        VoiceInterruptPeek = 0x1f
    };
}

// SVII—Synthesizer Voices IRQ

// Address: P3XR+5h read; index IGIDXR=8Fh read

// Default: E0h

// This register indicates which voice needs interrupt service and what type of interrupt service
// is needed. Indexing this register with a value of 8Fh in the General Index register (IGIDXR)
// clears the IRQ bits in the voice-specific Synthesizer Volume Control or Synthesizer Address
// Control registers that caused the interrupt and also clears the Volume Loop IRQ and
// Address Loop IRQ bits in the IRQ Status register (UISR[6:5]).

// +------------+------------+------------+------------+------------+------------+------------+------------+
// | Bit 7      | Bit 6      | Bit 5      | Bit 4      | Bit 3      | Bit 2      | Bit 1      | Bit 0      |
// +------------+------------+------------+------------+------------+------------+------------+------------+
// | Wavetable  | Volume IRQ | reserved   | Voice Number                                                   |
// | IRQ        |            |            |                                                                |
// +------------+------------+------------+------------+------------+------------+------------+------------+

// Bit 7:      Wavetable IRQ. When Low, the voice indicated in the Voice Number field
//             has crossed an address boundary and has caused an interrupt.

// Bit 6:      Volume IRQ. When Low, the voice indicated in the Voice Number field has
//             crossed a volume boundary and has caused an interrupt.

// Bit 5:      Reserved.

// Bits 4–0:   Voice Number. These bits indicate which voice needs interrupt service.

// Note: All bits in this register except reserved bits are self-modifying.


// SVIRI—Synthesizer Voices IRQ Read

// Address: P3XR+5h read; index IGIDXR=9Fh read

// Default: E0h

// This register contains the same bits as the SVII register but can be read without clearing
// any internally stored interrupt conditions.


typedef Indexed::Register8_t VoiceInterrupt_t;
namespace VoiceInterrupt
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

    inline VoiceInterrupt_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::VoiceInterrupt) + uint8_t(Register::BaseRead));
    }

    inline VoiceInterrupt_t Peek(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::VoiceInterruptPeek) + uint8_t(Register::BaseRead));
    }
}

}