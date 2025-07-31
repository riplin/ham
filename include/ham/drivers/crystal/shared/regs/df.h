//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        DataFormat = 0x08,
    };
}

// Data Format Register (Index 8)

// +------+------+------+------+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+------+------+------+------+------+
// | res  | FMT  | C/L  | S/M  | CFS2   CFS1   CFS0 | CSL  |
// +------+------+------+------+------+------+------+------+

// CSL         Clock Source Select: These bits select
//             the clock source used for the audio
//             sample rates.
//             CAUTION: See note at end of this
//             section about changing these bits
//                 0 - XTAL1/24.576 MHz
//                 1 - XTAL2/16.9344 MHz

// Note: When only one crystal or clock source
// is provided in hardware,it must be XTAL1.

// CFS2-CFSO   Clock Frequency Divide Select: These
//             bits select the audio sample rate
//             frequency. The actual audio sample
//             rate depends on which Clock Source
//             is selected and it's frequency.
//             CAUTION: See note below
//             about changing bits

//             XTAL1       XTAL2
// Divide      24.576 MHz  16.9344 MHz
// 0 - 3072    8.0 kHz     5.51 kHz
// 1 - 1536    16.0 kHz    11.025 kHz
// 2 - 896     27.42 kHz   18.9 kHz
// 3 - 768     32.0 kHz    22.05 kHz
// 4 - 448     N/A         37.8 kHz
// 5 - 384     N/A         44.1 kHz
// 6 - 512     48.0 kHz    33.075 kHz
// 7 - 2560    9.6 kHz     6.62 KHZ:

// S/M         Stereo/Mono Select: This bit
//             determines how the audio data streams
//             are formatted. Selecting stereo wil
//             result with alternating samples
//             representing left and right audio
//             channels. Mono playback plays the
//             same audio sample on both channels.
//             Mono capture only captures data from
//             the left audio channel.
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

// This register's initial state after reset is: x0000000.

// Note: The Contents of this register CANNOT be
// changed except when the CS4248 is in Mode
// Change Enable (MCE) is 1. If MCE is not one,
// writes to this register will be ignored.

typedef uint8_t DataFormat_t;
namespace DataFormat
{

    enum
    {
        ClockSource = 0x01,
        Clock24p576 = 0x00,
        Clock16p9344 = 0x01,
        ClockFrequencyDivideSelect = 0x0E,
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
            ClockSource = 0x00,
            ClockFrequencyDivideSelect = 0x01,
            Channels = 0x04,
            SignalType = 0x05,
            Format = 0x06,
        };
    };

}

}