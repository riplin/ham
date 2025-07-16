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
        ActiveVoices = 0x0e
    };
}

// Bits 7-6    Must be set to a 1
// Bits 5-0    # of voices to enable - 1.

// The range is from 14 - 32. Any value less than 14 will be forced to 14.

typedef Indexed::Register8_t ActiveVoices_t;
namespace ActiveVoices
{

    enum
    {
        VoicesMinusOne = 0x3f,
        MustBeSet = 0xc0
    };

    namespace Shift
    {
        enum
        {
            VoicesMinusOne = 0x00,
            MustBeSet = 0x06
        };
    }

    inline ActiveVoices_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::ActiveVoices) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, ActiveVoices_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::ActiveVoices) + uint8_t(Register::BaseWrite), value | MustBeSet);
    }

}

}