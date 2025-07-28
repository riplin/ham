//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        VibratoAndFilterModulationFromLfo1 = 0x03
    };
}

// Bits 15-8 of this word register are programmed with the degree and sign with which the
// channel’s LFO #1 provides vibrato. A value of 0x00 provides no effect, and positive values
// cause increasing vibrato with peak deviation of +/-1 octave at value 0x7f and -/+1 octave at
// 0x80.

// Bits 7-0 are programmed with the degree and sign with which the channel’s LFO #1 affects its
// filter cutoff, with 0x00 providing no effect, and positive values first increasing then decreasing
// cutoff, with peak deviation of +/-3 octaves at 0x7f and -/+3 octaves at 0x80.

typedef uint16_t VibratoAndFilterModulationFromLfo1_t;
namespace VibratoAndFilterModulationFromLfo1
{

    enum
    {
        Lfo1FilterCutoffEffect = 0x00FF,
        LFo1VibratoEffect = 0xFF00
    };

    namespace Shift
    {
        enum
        {
            Lfo1FilterCutoffEffect = 0x00,
            LFo1VibratoEffect = 0x08
        };
    }

    inline VibratoAndFilterModulationFromLfo1_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read3Word(baseAddress, Register::VibratoAndFilterModulationFromLfo1, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, VibratoAndFilterModulationFromLfo1_t value)
    {
        Data::Write3Word(baseAddress, Register::VibratoAndFilterModulationFromLfo1, channel, value);
    }

}

}