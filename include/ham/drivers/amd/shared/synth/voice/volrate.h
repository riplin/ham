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
        VolumeRampRate = 0x06,
    };
}

// SVRI—Synthesizer Volume Rate

// Address:    P3XR+5h read/write; index IGIDXR=06h write or IGIDXR=86h read; voice
//             index SVSR=(00h through 1Fh)

// Default: 00h

// This register controls the rate at which the looping volume for a voice is incremented and
// the amount of the increment.

// +------------+------------+------------+------------+------------+------------+------------+------------+
// | Bit 7      | Bit 6      | Bit 5      | Bit 4      | Bit 3      | Bit 2      | Bit 1      | Bit 0      |
// +------------+------------+------------+------------+------------+------------+------------+------------+
// | Volume Rate             | Volume Increment                                                            |
// |                         |                                                                             |
// +------------+------------+------------+------------+------------+------------+------------+------------+

// Bits 7–6:   Volume Rate. The rate at which the increment adds to the volume and the
//             division of the increment value.
//                 0:add volume increment every frame
//                 1:add (volume increment )/8 every frame
//                 2:add (volume increment )/8 every 8th frame
//                 3:add (volume increment )/8 every 64th frame

// Bits 5–0:   Volume Increment. The amount of increment.

typedef Indexed::Register8_t VolumeRampRate_t;
namespace VolumeRampRate
{

    enum
    {
        Increment = 0x3F,
        Rate = 0xC0
    };

    namespace Shift
    {
        enum
        {
            Increment = 0x00,
            Rate = 0x06
        };
    }

    inline VolumeRampRate_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::VolumeRampRate) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, VolumeRampRate_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::VolumeRampRate) + uint8_t(Voice::Register::BaseWrite), value);
    }

}

}