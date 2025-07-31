//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        TestAndInitialization = 0x0B,
    };
}

// Test and Initialization Register (Index 11)

// +------+------+------+-----+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4 |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+-----+------+------+------+------+
// | COR  | PUR  | ACI  | DRS | ORR1   ORR0 | ORL1   ORL0 |
// +------+------+------+-----+------+------+------+------+

// ORL1-ORLO   Overrange Left Detect: These bits
//             determine the overrange on the left
//             input channel. (Read Only) These bits
//             hold the peak value and are reset to "0"
//             by a read of this register
//                 0 - Greater than -1.5 dB underrange
//                 1 - Between -1.5 dB and 0 dB
//                     underrange
//                 2 - Between 0 dB and 1.5 dB
//                     overrange
//                 3 - Greater 1.5 dB overrange

// ORR1-ORRO   Overrange Right Detect: These bits
//             determine the overrange on the right
//             input channel. (Read Only) These bits
//             hold the peak value and are reset to
//             "0" by a read of this register
//                 0 - Greater than -1.5 dB underrange
//                 1 - Between -1.5 dB and 0 dB
//                     underrange
//                 2 - Between 0 dB and 1.5 dB overrange
//                 3 - Greater than 1.5 dB overrange

// DRS         DRQ Status: This bit indicates the
//             current status of the PDRQ and CDRQ
//             pins of the CS4248.
//                 0 - CDRQ AND PDRQ are presently
//                     inactive
//                 1 - CDRQ OR PDRQ are presently
//                     active

// ACI         Auto calibrate In-Progress: This bit
//             indicates the state of auto calibration
//             (Read-Only)
//                 0 - Auto calibration not in progress
//                 1 - Auto calibration is in progress

// PUR         Playback underrun: This bit is set when
//             playback data has not arrived from the
//             host in time to be played. As a result
//             the last valid sample will be sent to the
//             DAC. This bit is sticky and will set in
//             in an error condition. This bit is
//             cleared by a Status register read.

// CUR         Capture overrun: This bit is set when
//             the capture data has not been read by
//             the host before the next sample arrives.
//             The sample being read will not be
//             overwritten by the new sample. The
//             new sample will be ignored. This bit is
//             sticky and will stay set in an error
//             condition. This bit is cleared by a
//             Status register read.

// The SER bit in the Status register is simply a
// logical OR of the COR and PUR bits. This en-
// ables a polling host CPU to detect an error
// condition while checking other status bits.

// This register's initial state after reset is: 00000000

typedef uint8_t TestAndInitialization_t;
namespace TestAndInitialization
{

    enum
    {
        OverrangeLeft = 0x03,
        OverrangeRight = 0x0C,
        DataRequestStatus = 0x10,
        DataRequestsInactive = 0x00,
        DataRequestsActive = 0x10,
        AutoCalibrateStatus = 0x20,
        AutoCalibrateInactive = 0x00,
        AutoCalibrateActive = 0x20,
        PlaybackStatus = 0x40,
        PlaybackHealthy = 0x00,
        PlaybackUnderrun = 0x40,
        CaptureStatus = 0x80,
        CaptureHealthy = 0x00,
        CaptureOverrun = 0x80
    };

    namespace Shift
    {
        enum
        {
            OverrangeLeft = 0x03,
            OverrangeRight = 0x0C,
            DataRequestStatus = 0x10,
            AutoCalibrateStatus = 0x20,
            PlaybackStatus = 0x40,
            CaptureStatus = 0x80
        };
    };

}

}