//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/amd/shared/codec/regtype.h>

namespace Ham::Amd::Shared::Codec
{

namespace Register
{
    enum
    {
        LeftMicrophoneInputControl = 0x16,
        RightMicrophoneInputControl = 0x17
    };
}

// CLMICI, CRMICI—Left/Right Microphone Input Control

// Address:    PCODAR+1 read, write; left index CIDXR[4:0]=16h, right index
//             CIDXR[4:0]=17h

// Default: 1XX0 1000 (for both)

// Modes: 3

// This register pair controls the left and right MIC inputs to the mixer. The registers are
// identical; one controls the left channel and the other controls the right channel.

// +------------------+------------------+----------------+----------------+----------------+----------------+----------------+------------+
// | Bit 7            | Bit 6            | Bit 5          | Bit 4          | Bit 3          | Bit 2          | Bit 1          | Bit 0      |
// +------------------+------------------+----------------+----------------+----------------+----------------+----------------+------------+
// | Microphone       | reserved         | reserved       | Microphone gain select                                                         |
// | Mute Enable      |                  |                |                                                                                |
// |                  |                  |                |                                                                                |
// +------------------+------------------+----------------+----------------+----------------+----------------+----------------+------------+

// Bit 7:      Left/Right MIC Mute Enable. If set High, the MIC input is muted. If set Low,
//             the input operates normally.

// Bits 6–5:   Reserved.

// Bits 4–0:   Left/Right MIC Gain Select. These bits specify the amount of gain applied
//             to the MIC[L,R] input signals. The values vary from 0 = +12 dB to 1Fh =
//             –34.5 dB with 1.5 dB per step.



typedef Crystal::CS4231::Data_t MicrophoneInputControl_t;
namespace MicrophoneInputControl
{
    enum
    {
        Gain = 0x1F,
        Mute = 0x80
    };
}

namespace IO::MicrophoneInputControl
{
    inline MicrophoneInputControl_t ReadLeft(Register_t baseAddress)
    {
        return Crystal::CS4231::Data::Read(baseAddress, Register::LeftMicrophoneInputControl);
    }

    inline MicrophoneInputControl_t ReadRight(Register_t baseAddress)
    {
        return Crystal::CS4231::Data::Read(baseAddress, Register::RightMicrophoneInputControl);
    }
    
    inline void WriteLeft(Register_t baseAddress, MicrophoneInputControl_t value)
    {
        Crystal::CS4231::Data::Write(baseAddress, Register::LeftMicrophoneInputControl, value);
    }

    inline void WriteRight(Register_t baseAddress, MicrophoneInputControl_t value)
    {
        Crystal::CS4231::Data::Write(baseAddress, Register::RightMicrophoneInputControl, value);
    }
}

}