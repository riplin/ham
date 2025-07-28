//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        VolumeAndFilterCutoffTargets = 0x03
    };
}

// Bits 31-16 of this doubleword register determine the value to which the current volume of the
// channel will slew.

// Bits 15-0 determine the current value to which the current filter cutoff will slew. This register is
// being constantly updated by the envelope engine, so it is generally not written.

typedef uint32_t VolumeAndFilterCutoffTargets_t;
namespace VolumeAndFilterCutoffTargets
{

    enum
    {
        FilterCutoffTarget = 0x0000FFFF,
        VolumeTarget = 0xFFFF0000
    };

    namespace Shift
    {
        enum
        {
            FilterCutoffTarget = 0x00,
            VolumeTarget = 0x10
        };
    }

    inline VolumeAndFilterCutoffTargets_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read0DWord(baseAddress, Register::VolumeAndFilterCutoffTargets, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, VolumeAndFilterCutoffTargets_t value)
    {
        Data::Write0DWord(baseAddress, Register::VolumeAndFilterCutoffTargets, channel, value);
    }

}

}