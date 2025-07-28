//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Global
{

namespace Register
{
    enum
    {
        SoundMemoryAddressLeftRead = 0x01,
        SoundMemoryAddressRightRead = 0x01
    };
}

// Bit 31 of this doubleword register is an “empty” bit, which indicates whether register SMLD is
// empty or full of data for reading. When cleared indicates the DMA data has been read from the
// sound memory into the SMLD register and can be read. Read only.

// Bits 31-24 are Don’t Care on write, and bits 30-24 are zero on read.

// Bits 23-0 are the sound memory address which will be used for “left” DMA stream data fetch
// the next time the SMLD read register becomes empty and a DMA channel is available to fill it.

typedef uint32_t SoundMemoryAddress_t;
namespace SoundMemoryAddressRead
{
    enum
    {
        SoundMemoryAddress = 0x00FFFFFF,
        Satus = 0x80000000,
        Empty = 0x80000000,
        Full = 0x00000000,

        DramBaseAddress = (4 * 1024 * 1024) >> 1 // First 4MB is ROM, word adressed.
    };

    namespace Shift
    {
        enum
        {
            SoundMemoryAddress = 0x00,
            Satus = 0x1f,
        };
    }

    inline SoundMemoryAddress_t ReadLeft(Register_t baseAddress)
    {
        return Data::Read1DWord(baseAddress, Register::SoundMemoryAddressLeftRead, 20);
    }

    inline SoundMemoryAddress_t ReadRight(Register_t baseAddress)
    {
        return Data::Read1DWord(baseAddress, Register::SoundMemoryAddressRightRead, 21);
    }

    inline void WriteLeft(Register_t baseAddress, SoundMemoryAddress_t value)
    {
        Data::Write1DWord(baseAddress, Register::SoundMemoryAddressLeftRead, 20, value);
    }

    inline void WriteRight(Register_t baseAddress, SoundMemoryAddress_t value)
    {
        Data::Write1DWord(baseAddress, Register::SoundMemoryAddressRightRead, 21, value);
    }

}

}