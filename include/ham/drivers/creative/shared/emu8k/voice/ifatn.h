//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        InitialFilterCutoffAndAttenuation = 0x01
    };
}

// Bits 15-8 of this word register are programmed with the channel’s initial filter cutoff value. The
// value is in quarter semitones, with 0x00 being 125 Hz and 0xFF being 8 kHz. If the Q of the
// channel is programmed to zero and the filter cutoff to 0xFF, the filter does not alter the signal.

// Bits 7-0 of are programmed with the channel’s attenuation in 0.375 dB steps, with 0x00 being no
// attenuation and 0xFF being 96 dB.

typedef uint16_t InitialFilterCutoffAndAttenuation_t;
namespace InitialFilterCutoffAndAttenuation
{

    enum
    {
        Attenuation = 0x00FF,
        FilterCutoff = 0xFF00,
        NoFilter = 0xFF00
    };

    namespace Shift
    {
        enum
        {
            Attenuation = 0x00,
            FilterCutoff = 0x08
        };
    }

    inline InitialFilterCutoffAndAttenuation_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read3Word(baseAddress, Register::InitialFilterCutoffAndAttenuation, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, InitialFilterCutoffAndAttenuation_t value)
    {
        Data::Write3Word(baseAddress, Register::InitialFilterCutoffAndAttenuation, channel, value);
    }

}

}