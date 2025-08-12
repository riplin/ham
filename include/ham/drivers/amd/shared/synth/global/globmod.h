//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/amd/shared/synth/data.h>
#include <ham/drivers/amd/shared/synth/voice/vocbase.h>

namespace Ham::Amd::Shared::Synthesizer::Global
{

namespace Register
{
    enum
    {
        GlobalMode = 0x19
    };
}

// SGMI—Synthesizer Global Mode

// Address: P3XR+5h read/write; index IGIDXR=19h write or IGIDXR=99h read

// Default: 00h

// This register controls modes of operation that affect all voices.

// +------------+------------+------------+------------+------------+------------+------------+------------+
// | Bit 7      | Bit 6      | Bit 5      | Bit 4      | Bit 3      | Bit 2      | Bit 1      | Bit 0      |
// +------------+------------+------------+------------+------------+------------+------------+------------+
// | reserved   | reserved   | reserved   | reserved   | reserved   | reserved   | Global LFO | Enhanced   |
// |            |            |            |            |            |            | Enable     | Mode       |
// +------------+------------+------------+------------+------------+------------+------------+------------+

// Bits 7–4: Reserved.

// Bit 3:      Reserved. This bit must be written to 0.

// Bit 2:      Reserved. This bit must be written to 0.

// Bit 1:      Global LFO Enable. If set High, enables operation of all LFOs.

// Bit 0:      Enhanced Mode. If set High, enables enhanced features added to the GUS
//             capabilities.

typedef Indexed::Register8_t GlobalMode_t;
namespace GlobalMode
{

    enum
    {
        EnhancedModeControl = 0x01,
        EnhancedModeDisable = 0x00,
        EnhancedModeEnable = 0x01,
        GlobalLfoControl = 0x02,
        GlobalLfoDisable = 0x00,
        GlobalLfoEnable = 0x02
    };

    namespace Shift
    {
        enum
        {
            EnhancedModeControl = 0x00,
            GlobalLfoControl = 0x01
        };
    }

    inline GlobalMode_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::GlobalMode) + uint8_t(Voice::Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, GlobalMode_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::GlobalMode) + uint8_t(Voice::Register::BaseWrite), value);
    }

}

}