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
        PanPosition = 0x0c
    };
}

// Bits 7-4    Reserved (Set to 0)
// Bits 3-0    Pan position. (0=full left, 15=full right)

typedef Indexed::Register8_t PanPosition_t;
namespace PanPosition
{

    enum
    {
        Position = 0x0f
    };

    namespace Shift
    {
        enum
        {
            Position = 0x00
        };
    }

    inline PanPosition_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::PanPosition) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, PanPosition_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::PanPosition) + uint8_t(Register::BaseWrite), value);
    }

}

}