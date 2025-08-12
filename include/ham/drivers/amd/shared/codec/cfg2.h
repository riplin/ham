//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/amd/shared/codec/regtype.h>

namespace Ham::Amd::Shared::Codec
{

// CFIG2I—Configuration Register 2

// Address: PCODAR+1 read, write; index CIDXR[4:0]=10h

// Default: 0000 XXX0

// Modes: 2 and 3

// This register selects the full-scale voltage output range, enables the codec timer, enables
// the record and playback sample counters, and enables DAC output forcing.

// +---------------+----------------+----------------+----------------+----------------+----------------+----------------+------------+
// | Bit 7         | Bit 6          | Bit 5          | Bit 4          | Bit 3          | Bit 2          | Bit 1          | Bit 0      |
// +---------------+----------------+----------------+----------------+----------------+----------------+----------------+------------+
// | Output Full-  | Timer Enable   | Record Sample  | Playback       | Reserved       | Reserved       | Reserved       | DAC Output |
// | Scale Voltage |                | Counter        | Sample Counter |                |                |                | Force      |
// | Select        |                | Disable        | Disable        |                |                |                | Enable     |
// +---------------+----------------+----------------+----------------+----------------+----------------+----------------+------------+

// Bit 7:      Output Full-Scale Voltage Select. If set High, the full-scale output is 2.9
//             V for Vcc = 5 V and 1.34 for Vcc = 3.3 V. If set Low, the full-scale output is
//             2.0 V for Vcc = 5 V and 1.00 for Vcc = 3.3 V. This bit affects the left and right
//             signals that exit the mixers, prior to entering the Left and Right Output
//             Attenuation registers (CLOAI and CROAI); therefore, it also affects the input
//             to the record multiplexer.

// Bit 6:      Timer Enable. If set High, the timer and its associated interrupt are enabled.
//             If set Low, the timer is disabled. The timer count is specified in the Lower
//             Timer and Upper Timer registers (CLTIMI and CUTIMI).

// Bit 5:      Record Sample Counter Disable. If set High, this bit disables the record
//             sample counter from counting. This bit can be accessed in mode 3 only and
//             affects only the sample counter in mode 3.

// Bit 4:      Playback Sample Counter Disable. If set High, this bit disables the
//             playback sample counter from counting. This bit can be accessed in mode
//             3 only and affects only the sample counter in mode 3.

// Bits 3–1:   Reserved.

// Bit 0:      DAC Output Force Enable. If set High, the output of the DACs are forced
//             to the center of the scale whenever a playback FIFO underrun error occurs.
//             When cleared, the last valid sample is output in the event of an underrun.

typedef Crystal::CS4231::AlternateFeatureEnableI_t Configuration2_t;
namespace Configuration2
{
    enum
    {
        DacControl = Crystal::CS4231::AlternateFeatureEnableI::DacControl,
        DacHoldPrevious = Crystal::CS4231::AlternateFeatureEnableI::DacHoldPrevious,
        DacCenterScale = Crystal::CS4231::AlternateFeatureEnableI::DacCenterScale,
        PlaybackSampleCounterControl = 0x10,
        PlaybackSampleCounterEnable = 0x00,
        PlaybackSampleCounterDisable = 0x10,
        RecordSampleCounterControl = 0x20,
        RecordSampleCounterEnable = 0x00,
        RecordSampleCounterDisable = 0x20,
        TimerControl = Crystal::CS4231::AlternateFeatureEnableI::TimerControl,
        TimerDisable = Crystal::CS4231::AlternateFeatureEnableI::TimerDisable,
        TimerEnable = Crystal::CS4231::AlternateFeatureEnableI::TimerEnable,
        OutputLevel = Crystal::CS4231::AlternateFeatureEnableI::OutputLevel,
        OutputLevel1VppAt3p3V = Crystal::CS4231::AlternateFeatureEnableI::OutputLevel2Vpp,
        OutputLevel1p34VppAt3p3V = Crystal::CS4231::AlternateFeatureEnableI::OutputLevel2p8Vpp,
        OutputLevel2VppAt5V = Crystal::CS4231::AlternateFeatureEnableI::OutputLevel2Vpp,
        OutputLevel2p9VppAt5V = Crystal::CS4231::AlternateFeatureEnableI::OutputLevel2p8Vpp
    };
}

namespace IO::Configuration2
{
    inline Configuration2_t Read(Register_t baseAddress)
    {
        return Crystal::CS4231::IO::AlternateFeatureEnableI::Read(baseAddress);
    }
    
    inline void Write(Register_t baseAddress, Configuration2_t value)
    {
        Crystal::CS4231::IO::AlternateFeatureEnableI::Write(baseAddress, value);
    }
}

}