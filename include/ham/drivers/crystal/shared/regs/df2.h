//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/df.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        DataFormat2 = DataFormat,
    };
}

// Data Format Register (Index 8)

// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | FMT2 | FMT1 | C/L  | S/M  | CFS2   CFS1   CFS0 | CSL  |
// +------+------+------+------+------+------+------+------+

// C2SL        Clock 2 Source Select: This bit selects
//             the clock source used for the audio
//             sample rates for both capture and
//             playback.
//             CAUTION: See note below about
//             changing these bits
//             0 - XTAL1 Typically 24.576 MHz
//             1 - XTAL2 Typically 16.9344 MHz

// CFS2-CFSO   Clock Frequency Divide Select: These
//             bits select the audio sample fre-
//             quency for both capture and
//             playback. The actual audio sample
//             frequency depends on which clock
//             source (C2SL) is selected and its fre-
//             quency. Frequencies listed as N/A
//             are not available because their sam-
//             ple frequency violates the maximum
//             specifications; however, the decodes
//             are available and may be used with
//             crystals that do not violate the sam-
//             ple frequency specifications.
//             CAUTION: See note below about
//             changing bits

//             XTAL1           XTAL2
// Divide      24.576 MHz      16.9344 MHz
// 0 - 3072     8.0 kHz         5.51 kHz
// 1 - 1536    16.0 kHz        11.025 kHz
// 2 - 896     27.42 kHz       18.9 kHz
// 3 - 768     32.0 kHz        22.05 kHz
// 4 - 448      N/A            37.8 kHz
// 5 - 384      N/A            44.1 kHz
// 6 - 512     48.0 kHZ        33.075 kHz
// 7 - 2560     9.6 kHz         6.62 kHz

// S/M         Stereo/Mono Select: This bit deter-
//             mines how the audio data streams
//             are formatted. Selecting stereo will
//             result in alternating samples repre-
//             senting left and right audio channels.
//             Mono playback plays the same
//             audio sample on both channels.
//             Mono capture only captures data
//             from the left channel. In MODE 1,
//             this bit is used for both playback and
//             capture. In MODE 2, this bit is only
//             used for playback, and the capture
//             format is independently selected via
//             128.
//             0 - Mono
//             1 - Stereo

// The C/L, FMT1, and FMTO bits set the audio data
// format as shown below. In MODE 1, FMT1, which is
// forced low, FMTO, and C/L are used for both play-
// back and capture. In MODE 2, these bits are only
// used for playback, and the capture format is inde-
// pendently selected via register 128.

// FMT1*   FMTO    C/L
//  D7      D6      D5
//  0       0       0      Linear, 8-bit unsigned
//  0       0       1      M-Law, 8-bit companded
//  0       1       0      Linear, 16-bit two's
//                         complement, Little Endian
//  0       1       1      A-Law, 8-bit companded
//  1       0       0      RESERVED
//  1       0       1      ADPCM, 4-bit, IMA compatible
//  1       1       0      Linear, 16-bit two's
//                         complement, Big Endian
//  1       1       1      RESERVED

// * FMT1 is not available in MODE 1 (forced to 0).

// This register's initial state after reset is: 0000000.

typedef uint8_t DataFormat2_t;
namespace DataFormat2
{

    enum
    {
        ClockSource = DataFormat::ClockSource,
        Clock24p576 = DataFormat::Clock24p576,
        Clock16p9344 = DataFormat::Clock16p9344,
        ClockFrequencyDivideSelect = DataFormat::ClockFrequencyDivideSelect,
        Channels = DataFormat::Channels,
        Mono = DataFormat::Mono,
        Stereo = DataFormat::Stereo,
        SignalType = DataFormat::SignalType,
        Linear = DataFormat::Linear,
        Companded = DataFormat::Companded,
        Format = DataFormat::Format | 0x80,
        Unsigned8Bit = DataFormat::Unsigned8Bit,
        Signed16bit = DataFormat::Signed16bit,
        uLaw = DataFormat::uLaw,
        aLaw = DataFormat::aLaw,
        Adpcm4bit = Companded | 0x80,
        Signed16BitBigEndian = 0xC0
    };

    namespace Shift
    {
        enum
        {
            ClockSource = DataFormat::Shift::ClockSource,
            ClockFrequencyDivideSelect = DataFormat::Shift::ClockFrequencyDivideSelect,
            Channels = DataFormat::Shift::Channels,
            SignalType = DataFormat::Shift::SignalType,
            Format = DataFormat::Shift::Format
        };
    };

}

}