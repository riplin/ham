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
        VolumeRampStart = 0x07,
        VolumeRampEnd = 0x08,
    };
}

// SVSI—Synthesizer Volume Start

// Address:    P3XR+5h read/write; index IGIDXR=07h write or IGIDXR=87h read; voice
//             index SVSR=(00h through 1Fh)

// Default: 00h

// This register contains the low point of a volume ramp.

// Bits 7–0:   Volume Start. An 8-bit value indicating the beginning volume of a volume
//             ramp.

// SVEI—Synthesizer Volume End

// Address:    P3XR+5h read/write; index IGIDXR=08h write or IGIDXR=88h read; voice
//             index SVSR=(00h through 1Fh)

// Default: 00h

// This register contains the high point of a volume ramp.

// Bits 7–0:   Volume End. An 8-bit value indicating the ending volume of a volume ramp.

typedef Indexed::Register8_t VolumeRamp_t;
namespace VolumeRamp
{

    inline VolumeRamp_t ReadStart(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::VolumeRampStart) + uint8_t(Register::BaseRead));
    }

    inline VolumeRamp_t ReadEnd(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::VolumeRampEnd) + uint8_t(Register::BaseRead));
    }

    inline void WriteStart(Register_t baseAddress, VolumeRamp_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::VolumeRampStart) + uint8_t(Voice::Register::BaseWrite), value);
    }

    inline void WriteEnd(Register_t baseAddress, VolumeRamp_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::VolumeRampEnd) + uint8_t(Voice::Register::BaseWrite), value);
    }

}

}