//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        Lfo1TremoloAmountAndFrequency = 0x04
    };
}

// Bits 15-8 of this word register are programmed with the degree and sign with which the
// channel’s LFO #1 provides tremolo. A value of 0x00 provides no effect, and positive values
// cause increasing then decreasing amplitude with peak deviation of +/-12 dB at value 0x7f and -
// /+12 dB at 0x80.

// Bits 7-0 are programmed to determine the channel’s LFO #1 frequency in 0.042 Hz steps with
// 0xFF equalling 10.72 Hz.

typedef uint16_t Lfo1TremoloAmountAndFrequency_t;
namespace Lfo1TremoloAmountAndFrequency
{

    enum
    {
        Frequency = 0x00FF,
        Amplitude = 0xFF00  //Signed
    };

    namespace Shift
    {
        enum
        {
            Frequency = 0x00,
            Amplitude = 0x08
        };
    }

    inline Lfo1TremoloAmountAndFrequency_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read3Word(baseAddress, Register::Lfo1TremoloAmountAndFrequency, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, Lfo1TremoloAmountAndFrequency_t value)
    {
        Data::Write3Word(baseAddress, Register::Lfo1TremoloAmountAndFrequency, channel, value);
    }

}

}