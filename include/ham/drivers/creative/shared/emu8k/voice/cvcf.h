//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Voice
{

namespace Register
{
    enum
    {
        CurrentVolumeAndFilterCutoff = 0x02
    };
}

// Bits 31-16 of this doubleword register show the current volume of the channel.

// Bits 15-0 show the current filter cutoff value of the channel. This register is constantly being
// overwritten with new data, so writing to it is generally pointless.

typedef uint32_t CurrentVolumeAndFilterCutoff_t;
namespace CurrentVolumeAndFilterCutoff
{

    enum
    {
        CurrentFilterCutoff = 0x0000FFFF,
        CurrentVolume = 0xFFFF0000
    };

    namespace Shift
    {
        enum
        {
            CurrentFilterCutoff = 0x00,
            CurrentVolume = 0x10
        };
    }

    inline CurrentVolumeAndFilterCutoff_t Read(Register_t baseAddress, Index_t channel)
    {
        return Data::Read0DWord(baseAddress, Register::CurrentVolumeAndFilterCutoff, channel);
    }

    inline void Write(Register_t baseAddress, Index_t channel, CurrentVolumeAndFilterCutoff_t value)
    {
        Data::Write0DWord(baseAddress, Register::CurrentVolumeAndFilterCutoff, channel, value);
    }

}

}