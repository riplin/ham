//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        PitchAndFilterEnvelopeHeights = 0x02
    };
}

// Bits 15-8 of this word register are programmed with the degree and sign with which the
// channel’s modulation envelope affects its pitch. A value of 0x00 provides no effect, and
// positive values cause increasing pitch with peak deviation of +1 octave at value 0x7f and -1
// octave at 0x80.

// Bits 7-0 are programmed with the degree and sign with which the channel’s modulation
// envelope affects its filter cutoff, with 0x00 providing no effect, and positive values increasing
// cutoff, with peak deviation of +6 octaves at 0x7f and -6 octaves at 0x80.

typedef uint16_t PitchAndFilterEnvelopeHeights_t;
namespace PitchAndFilterEnvelopeHeights
{

    enum
    {
        FilterCutoffHeight = 0x00FF,
        PitchHeight = 0xFF00
    };

    namespace Shift
    {
        enum
        {
            FilterCutoffHeight = 0x00,
            PitchHeight = 0x08
        };
    }

    inline PitchAndFilterEnvelopeHeights_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read3Word(baseAddress, Register::PitchAndFilterEnvelopeHeights, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, PitchAndFilterEnvelopeHeights_t value)
    {
        Data::Write3Word(baseAddress, Register::PitchAndFilterEnvelopeHeights, channel, value);
    }

}

}