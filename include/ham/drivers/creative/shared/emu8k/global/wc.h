//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Global
{

namespace Register
{
    enum
    {
        SampleCounter = 0x01,
    };
}

// This word register provides a counter continuously incrementing at the sample rate. There is no
// mechanism to reset this counter, which cycles through 65536 value every 1.486 seconds.

typedef uint16_t SampleCounter_t;
namespace SampleCounter
{

    inline SampleCounter_t Read(Register_t baseAddress)
    {
        return Data::Read2Word(baseAddress, Register::SampleCounter, 27);
    }

    inline void Write(Register_t baseAddress, SampleCounter_t value)
    {
        Data::Write2Word(baseAddress, Register::SampleCounter, 27, value);
    }

}

}