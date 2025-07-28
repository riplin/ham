//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        Lfo2VibratoAmountAndFrequency = 0x05
    };
}

// Bits 15-8 of this word register are programmed with the degree and sign with which the
// channel’s LFO #2 provides vibrato. A value of 0x00 provides no effect, and positive values
// cause increasing then decreasing pitch with peak deviation of +/-1 octave at value 0x7f and -/+1
// octave at 0x80.

// Bits 7-0 are programmed to determine the channel’s LFO #2 frequency in 00042 Hz steps with
// 0xFF equalling 10.72 Hz.

typedef uint16_t Lfo2VibratoAmountAndFrequency_t;
namespace Lfo2VibratoAmountAndFrequency
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

    inline Lfo2VibratoAmountAndFrequency_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read3Word(baseAddress, Register::Lfo2VibratoAmountAndFrequency, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, Lfo2VibratoAmountAndFrequency_t value)
    {
        Data::Write3Word(baseAddress, Register::Lfo2VibratoAmountAndFrequency, channel, value);
    }

}

}