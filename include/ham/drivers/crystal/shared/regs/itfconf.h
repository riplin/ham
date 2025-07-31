//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        InterfaceControl = 0x09,
    };
}

// Interface Configuration Register (Index 9)

// +------+------+------+-----+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4 |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+-----+------+------+------+------+
// | CPIO | PPIO | res  | res | ACAL | SDC  | CEN  | PEN  |
// +------+------+------+-----+------+------+------+------+

// PEN     Playback Enable. This bit enables
//         playback. The CS4248 will generate
//         PDRQ and respond to PDAK signals
//         when this bit is enabled and PPIO=0.
//         If PPIO=1, this bit enables PIO play-
//         back mode. PEN may be set and
//         reset without setting the MCE bit.
//             0 - Playback Disabled (PDRQ and
//                 PIO inactive)
//             1 - Playback Enabled

// CEN     Capture Enabled. This bit enables
//         the capture of data. The CS4248 will
//         generate CDRQ and respond to
//         CDAK signals when this bit is enablec
//         nables PIO capture mode. CEN may
//         be changed without setting the MC
//         bit.

//             0 - Capture Disabled (CDRQ and PIO
//                 inactive)
//             1 - Capture Enabled

// SDC     Single DMA Channel: This bit wil force
//         BOTH capture and playback DMA
//         requests to occur on the Playback DMA
//         channel. The Capture DMA CDRQ pin
//         will be zero. This bit will allow the
//         CS4248 to be used with only one DMA
//         channel. Should both capture and
//         playback be enabled in this mode, only
//         the playback will occur. See the DMA
//         section for further explanation.
//             0 - Dual DMA channel mode
//             1 - Single DMA channel mode

// ACAL    Auto calibrate Enable: This bit
//         determines whether the CS4248
//         performs an auto calibrate whenever
//         returning from the Mode Change Enable
//         (MCE) bit being asserted. If the ACAL
//         bit is not set, previous calibration
//         cycle are used, and no calibration
//         takes place. Therefore, ACAL is
//         normally set.
//             0 - No auto calibration
//             1 - Auto calibration allowed

// PPIO    Playback PIO Enable: This bit
//         determines whether the playback data
//         is transferred via DMA or PÍO.
//             0 - DMA transfers only
//             1 - PIO transfers only

// CPIO    Capture PIO Enable: This bit
//         Determines whether the capture data
//         is transferred via DMA or PIO.
//             0 - DMA transfers only
//             1 - PIO transfers only

// Note: This register, except bits CEN and PEN, car
// only be written while in Mode Change Enable. See
// section on MCE for more details.

// This register's initial state after reset is: 00xx1000

typedef uint8_t InterfaceControl_t;
namespace InterfaceControl
{

    enum
    {
        PlaybackControl = 0x01,
        PlaybackDisabled = 0x00,
        PlaybackEnabled = 0x01,
        CaptureControl = 0x02,
        CaptureDisabled = 0x00,
        CaptureEnabled = 0x02,
        DmaChannels = 0x04,
        DualDmaChannels = 0x00,
        SingleDmaChannel = 0x04,
        AutoCalibrateControl = 0x08,
        AutoCalibrateDisable = 0x00,
        AutoCalibrateEnable = 0x08,
        PlaybackTransferMode = 0x40,
        PlaybackTransferDma = 0x00,
        PlaybackTransferPio = 0x40,
        CaptureTransferMode = 0x80,
        CaptureTransferDma = 0x00,
        CaptureTransferPio = 0x80
    };

    namespace Shift
    {
        enum
        {
            PlaybackControl = 0x00,
            CaptureControl = 0x01,
            DmaChannels = 0x02,
            AutoCalibrateControl = 0x03,
            PlaybackTransferMode = 0x06,
            CaptureTransferMode = 0x07
        };
    };

}

}