//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        CaptureDataFormat = 0x1C,
    };
}

// Capture Data Format (I28)

// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | FMT1 | FMT0 | C/L  | S/M  | res  | res  | res  | res  |
// +------+------+------+------+------+------+------+------+

// S/M         Stereo/Mono Select: This bit deter-
//             mines how the capture audio data
//             stream is formatted. Selecting stereo
//             will result with alternating samples
//             representing left and right audio
//             channels. Selecting mono only cap-
//             tures data from the left audio
//             channel.
//                 0 - Mono
//                 1 - Stereo

// C/L         Companded /Linear Select: This bit
//             selects between a linear digital
//             representation of the audio signal or a
//             non-linear, companded format. The
//             type of companded format is defined
//             by the FMT bit.
//                 0 - Linear
//                 1 - Companded

// FMT         Format Select: This bit defines the
//             exact format of the digital audio based
//             on the state of the C/L bit.

// C/L=0                   C/L=1
// Linear                  Companded
// 0 - 8 bit, unsigned     0 - u-Law
// 1 - 16-bit, signed      1 - A-law

// The C/L, FMT1, and FMTO bits set the capture data
// format in MODE 2. See Table 10 or register 18 for
// the bit settings and data formats. The capture data
// format can be different that the playback data for-
// mat; however, the sample frequency must be the
// same and is set in 18. MCE must be set to modify
// this register.

// This register's initial state after reset is: 0000xxxx

typedef uint8_t CaptureDataFormat_t;
namespace CaptureDataFormat
{

    enum
    {
        Channels = 0x10,
        Mono = 0x00,
        Stereo = 0x10,
        SignalType = 0x20,
        Linear = 0x00,
        Companded = 0x20,
        Format = 0x40,
        Unsigned8Bit = 0x00,
        Signed16bit = 0x40,
        uLaw = Companded | 0x00,
        aLaw = Companded | 0x40
    };

    namespace Shift
    {
        enum
        {
            Channels = 0x04,
            SignalType = 0x05,
            Format = 0x06,
        };
    };

}

}