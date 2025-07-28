//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        VolumeEnvelopeDelay = 0x04
    };
}

// This word register is programmed with the channel’s volume envelope delay value at the
// beginning of the volume envelope. A value of 0x8000 indicates no delay; values below 0x8000
// indicate increasing delay in 725 usec units.

typedef uint16_t VolumeEnvelopeDelay_t;
namespace VolumeEnvelopeDelay
{

    enum
    {
        NoDelay = 0x8000,
        Delay725USec = 0x0001       //unit
    };

    inline VolumeEnvelopeDelay_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read1Word(baseAddress, Register::VolumeEnvelopeDelay, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, VolumeEnvelopeDelay_t value)
    {
        Data::Write1Word(baseAddress, Register::VolumeEnvelopeDelay, channel, value);
    }

}

}