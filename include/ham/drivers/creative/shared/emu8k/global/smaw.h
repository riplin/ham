//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Global
{

namespace Register
{
    enum
    {
        SoundMemoryAddressLeftWrite = 0x01,
        SoundMemoryAddressRightWrite = 0x01,
    };
}

// Bit 31 of this doubleword register is a “full” bit, which indicates whether register SMLD is
// empty or full of data for writing. When low, indicates the DMA data can be written to the
// SMRD register. Read only.

// Bits 31-24 are Don’t Care on write, and bits 30-24 are zero on read.

// Bits 23-0 are the sound memory address which will be used for “left” DMA stream data write if
// the SMLD write register is full and a DMA channel is available to write it.

typedef uint32_t SoundMemoryAddress_t;
namespace SoundMemoryAddressWrite
{

    enum
    {
        SoundMemoryAddress = 0x00FFFFFF,
        Satus = 0x80000000,
        Empty = 0x00000000,
        Full = 0x80000000,

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
        return Data::Read1DWord(baseAddress, Register::SoundMemoryAddressLeftWrite, 22);
    }

    inline SoundMemoryAddress_t ReadRight(Register_t baseAddress)
    {
        return Data::Read1DWord(baseAddress, Register::SoundMemoryAddressRightWrite, 23);
    }

    inline void WriteLeft(Register_t baseAddress, SoundMemoryAddress_t value)
    {
        Data::Write1DWord(baseAddress, Register::SoundMemoryAddressLeftWrite, 22, value);
    }

    inline void WriteRight(Register_t baseAddress, SoundMemoryAddress_t value)
    {
        Data::Write1DWord(baseAddress, Register::SoundMemoryAddressRightWrite, 23, value);
    }

}

}