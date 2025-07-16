//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/data.h>

namespace Ham::Gravis::Shared::GF1::Global
{

namespace Register
{
    enum
    {
        DmaStartAddress = 0x42
    };
}

// Bits 15-0 are Address lines 19-4.
// This register defines where the DMA will transfer data to or from. Since only the upper 16 address bits are used and
// the lower 4 bits are set to 0, a DMA transfer MUST begin on an 16 byte boundary for an 8 bit DMA channel (0-3). If
// a 16 bit DMA channel is being used, the transfer MUST begin on a 32 byte boundary. An additional address
// translation is necessary if a 16 bit DMA channel is used. For simple example code on how to do this translation, see
// the C function convert_to_16().

typedef Indexed::Register16_t DmaStartAddress_t;
namespace DmaStartAddress
{

    enum
    {
        Value = 0xffff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    inline DmaStartAddress_t Read(Register_t baseAddress)
    {
        return Data::Read16(baseAddress, Register::DmaStartAddress);
    }

    inline void Write(Register_t baseAddress, DmaStartAddress_t value)
    {
        Data::Write16(baseAddress, Register::DmaStartAddress, value);
    }

}

}
