//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/amd/shared/synth/data.h>
#include <ham/drivers/amd/shared/synth/voice/vocbase.h>

namespace Ham::Amd::Shared::Synthesizer::Global
{

namespace Register
{
    enum
    {
        LfoBaseAddress = 0x1A
    };
}

// SLFOBI—Synthesizer LFO Base Address

// This register holds the base address for the locations of voice LFO parameters.

// Address: P3XR+(4-5) read, write; index IGIDXR=1Ah write or IGIDXR=9Ah read

// Default: 0000h

// Bits 15–14:     Reserved.

// Bit 13–0:       LFO Base Address. Bits 23–10 of the base address in local memory for
//                 the locations of voice LFO parameters.

typedef Indexed::Register16_t LfoBaseAddress_t;
namespace LfoBaseAddress
{

    enum
    {
        Address = 0x3FFF
    };

    namespace Shift
    {
        enum
        {
            Address = 0x00
        };
    }

    inline LfoBaseAddress_t Read(Register_t baseAddress)
    {
        return Data::Read16(baseAddress, uint8_t(Register::LfoBaseAddress) + uint8_t(Voice::Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, LfoBaseAddress_t value)
    {
        Data::Write16(baseAddress, uint8_t(Register::LfoBaseAddress) + uint8_t(Voice::Register::BaseWrite), value);
    }

}

}