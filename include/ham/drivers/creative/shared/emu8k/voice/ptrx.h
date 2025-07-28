//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        PitchTargetReverbSendAndAux = 0x01
    };
}

// Bits 31-16 of this doubleword register determine the value to which the current pitch shift of the
// channel’s oscillator will slew.

// Bits 15-8 of the register determine the amount of Reverb Send from the channel to the effects
// engine, with 0x00 being none and 0xff being maximum.

// Bits 7-0 of the register are an auxilliary data byte generally unused. The upper 16 bits of this
// register are being constantly updated by the envelope engine. so it is wise to read this value and
// re-write the same value when changing the reverb send.

typedef uint32_t PitchTargetReverbSendAndAux_t;
namespace PitchTargetReverbSendAndAux
{

    enum
    {
        AuxData = 0x000000FF,
        ReverbSend = 0x0000FF00,
        PitchTarget = 0xFFFF0000
    };

    namespace Shift
    {
        enum
        {
            AuxData = 0x00,
            ReverbSend = 0x08,
            PitchTarget = 0x10
        };
    }

    inline PitchTargetReverbSendAndAux_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read0DWord(baseAddress, Register::PitchTargetReverbSendAndAux, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, PitchTargetReverbSendAndAux_t value)
    {
        Data::Write0DWord(baseAddress, Register::PitchTargetReverbSendAndAux, channel, value);
    }

}

}