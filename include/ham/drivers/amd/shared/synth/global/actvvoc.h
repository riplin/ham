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
        ActiveVoices = 0x0e
    };
}

// SAVI—Synthesizer Active Voices

// Address: P3XR+5h read/write; index IGIDXR=0Eh write or IGIDXR=8Eh read

// Default: CDh

// This register is needed only to remain compatible with the GUS. When the Enhanced Mode
// bit of the Synthesizer Global Mode register (SGMI[0]) is set High, this register does not
// affect operation. When SGMI[0] is Low, this register controls which voices produce an
// output and affect the output sample rate. The number of active voices can range from 14
// to 32. With 14 active voices, the output sample rate is 44.1 kHz or a sample period of
// approximately 22.7 µs. Each additional voice above 14 adds approximately 1.6 µs to the
// sample period. When SGMI[0] is Low, the frequency control values must be adjusted to
// compensate for the slower output sample rates when more than 14 voices are active. The
// programmed value equals the number of active voices minus 1. The programmed values
// of this register can range from CDh to DFh.

// +------------+------------+------------+------------+------------+------------+------------+------------+
// | Bit 7      | Bit 6      | Bit 5      | Bit 4      | Bit 3      | Bit 2      | Bit 1      | Bit 0      |
// +------------+------------+------------+------------+------------+------------+------------+------------+
// | reserved   | reserved   | reserved   | Active Voices                                                  |
// |            |            |            |                                                                |
// +------------+------------+------------+------------+------------+------------+------------+------------+

// Bits 7–5:   Reserved.

// Bits 4–0:   Active Voices. These bits indicate the number of active voices.

typedef Indexed::Register8_t ActiveVoices_t;
namespace ActiveVoices
{

    enum
    {
        VoicesMinusOne = 0x3F
    };

    namespace Shift
    {
        enum
        {
            VoicesMinusOne = 0x00
        };
    }

    inline ActiveVoices_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::ActiveVoices) + uint8_t(Voice::Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, ActiveVoices_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::ActiveVoices) + uint8_t(Voice::Register::BaseWrite), value);
    }

}

}