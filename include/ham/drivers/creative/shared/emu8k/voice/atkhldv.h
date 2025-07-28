//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        VolumeEnvelopeHoldAndAttack = 0x04
    };
}

// Bit 15 of this word register should be written as 0 to cause an envelope attack. 

// Bits 14-8 are the volume envelope hold time in 92 msec increments, with 0x7f being
// no hold time and 0x00 being 11.68 seconds. 

// Bit 7 should be written and read as zero.

// Bits 6-0 are the encoded volume envelope attack time, with 0x00 being never attack,
// 0x01 being 11.88 seconds, and 0x7f being 6 msec.

typedef uint16_t VolumeEnvelopeHoldAndAttack_t;
namespace VolumeEnvelopeHoldAndAttack
{

    enum
    {
        AttackTime = 0x007F,
        NeverAttack = 0x0000,
        Attack6msec = 0x007F,
        HoldTime = 0x7F00,
        HoldNever = 0x7F00,
        EnvelopeAttackControl = 0x8000,
        EnvelopeAttackDisable = 0x8000,
        EnvelopeAttackEnable = 0x0000
    };

    namespace Shift
    {
        enum
        {
            AttackTime = 0x00,
            HoldTime = 0x08,
            EnvelopeAttackControl = 0x0F
        };
    }

    inline VolumeEnvelopeHoldAndAttack_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read2Word(baseAddress, Register::VolumeEnvelopeHoldAndAttack, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, VolumeEnvelopeHoldAndAttack_t value)
    {
        Data::Write2Word(baseAddress, Register::VolumeEnvelopeHoldAndAttack, channel, value);
    }

}

}