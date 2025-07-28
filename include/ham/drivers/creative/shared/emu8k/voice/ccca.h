//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        QControlBitsAndCurrentAddress = 0x00
    };
}

// Bits 31-28 of this doubleword register determine the filter Q of the channel, with 0 being no
// resonance, and 15 being about 24 dB of resonance.

// Bit 27 should always be zero.

// Bit 26 is the DMA bit for the channel, which should only be set when the channel is being used
// for sound memory access.

// Bit 25 is the WR bit, which should only be set when bit 26 is set, and determines if the DMA
// channel is read or write. 1=write, 0 = read.

// Bit 24 is the RIGHT bit for the channel which should be only set when bit 26 is set, and
// determines if the DMA channel uses the “left” or “right” DMA stream. 1=right, 0=left.

// Bits 23-0 are the current sound memory address for the channel. Note that the actual audio
// location is the point 1 word higher than this value due to interpolator offset.

typedef uint32_t QControlBitsAndCurrentAddress_t;
namespace QControlBitsAndCurrentAddress
{

    enum
    {
        CurrentSoundMemoryAddress = 0x00FFFFFF,
        Channel = 0x01000000,
        Left = 0x00000000,
        Right = 0x01000000,
        Access = 0x02000000,
        ReadAccess = 0x00000000,
        WriteAccess = 0x00000000,
        Type = 0x0400000000,
        Sound = 0x00000000,
        Dma = 0x04000000,
        Q = 0xF0000000
    };

    namespace Shift
    {
        enum
        {
            CurrentSoundMemoryAddress = 0x00,
            Channel = 0x18,
            Access = 0x19,
            Type = 0x1a,
            Q = 0x1c
        };
    }

    inline QControlBitsAndCurrentAddress_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read1DWord(baseAddress, Register::QControlBitsAndCurrentAddress, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, QControlBitsAndCurrentAddress_t value)
    {
        Data::Write1DWord(baseAddress, Register::QControlBitsAndCurrentAddress, channel, value);
    }

}

}