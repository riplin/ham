//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <ham/drivers/crystal/regtype.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        Status = 0x02
    };
}

// +------+------+------+-----+------+------+------+-----+
// |  D7  |  D6  |  D5  | D4  |  D3  |  D2  |  D1  |  DO |
// +------+------+------+-----+------+------+------+-----+
// | CU/L | CL/R | CRDY | SER | PU/L | PL/R | PRDY | INT |
// +------+------+------+-----+------+------+------+-----+

// INT     Interrupt Status: This indicates the
//         status of the internal interrupt logic of
//         the CS4248. This bit is cleared by any
//         write of any value to this register. The
//         TEN bit of the Pin Control register deter-
//         mines whether the state of this bit is
//         reflected on the IRQ pin of the CS4248.
//         Read States

//             0 - Interrupt pin inactive
//             1 - Interrupt pin active

// PRDY    Playback Data Register Ready. The
//         Playback Data register is ready for
//         more data. This bit would be used when
//         direct programmed 1/0 data transfers
//         are desired. (This bit is Read-Only)

//             0 - Data still valid. Do not overwrite.
//             1 - Data stale. Ready for next host
//                 data write value.

// PL/R    Playback Right/Left Sample: This bit
//         indicates whether data needed is for
//         the Right channel or Left channel.
//         (This bit is Read-Only)

//             0 - Right Channel Data
//             1 - Left Channel Data or Mono selected

// PU/L    Playback Upper/Lower Byte: This bit
//         indicates whether the playback data
//         needed is for the upper or lower byte of
//         the channel. (This bit is Read-Only)

//             0 - Lower Byte Needed
//             1 - Upper Byte Needed or any
//                 8-bit mode

// SER     Sample Error: This bit indicates
//         that a sample was not serviced in time
//         and therefore an error has occurred.
//         The bit indicates an overrun for capture
//         and underrun for playback. If both the
//         capture and playback are enabled, the
//         source which set this bit can not be
//         determined. (This bit is Read-Only)

// CRDY    Capture Data Ready. The Capture
//         Data register contains data ready for
//         reading by the host. This bit would be
//         used for direct programmed 1/0 data
//         transfers. (This bit is a Read-Only)

//             O - Data is stale. Do not reread the
//                 information.
//             1 - Data is fresh. Ready for next host
//                 data read.

// CL/R    Capture Left/Right Sample: This bit
//         indicates whether the capture data
//         waiting is for the Right channel or Left
//         channel. (This bit is Read-Only)

//             0 - Right Channel Data
//             1 - Left Channel Data or mono

// CU/L    Capture Upper/Lower Byte: This bit
//         indicates whether the capture data
//         ready is for the upper or lower byte of
//         the channel. (This bit is Read-Only)

//             0 - Lower byte ready
//             1 - Upper byte ready or any 8-bit mode

// The PRDY and CRDY bits are designed to be
// read as one when action is required by the host.
// For example, when PRDY is set to one the de-
// vice is ready for more data, or when the CRDY
// is set to one data is available to the host. The
// definition of the CRDY and PRDY bits are
// therefore consistent in this regard.

typedef uint8_t Status_t;
namespace Status
{
    enum
    {
        InterruptStatus = 0x01,
        InterruptClear = 0x00,
        InterruptPending = 0x01,
        PlaybackDataStatus = 0x02,
        PlaybackDataReady = 0x00,
        PlaybackDataStale = 0x02,
        PlaybackChannel = 0x04,
        PlaybackRight = 0x00,
        PlaybackLeft = 0x04,
        PlaybackByte = 0x08,
        PlaybackLower = 0x00,
        PlaybackUpper = 0x08,
        SampleStatus = 0x10,
        SampleOk = 0x00,
        SampleError = 0x10,
        CaptureStatus = 0x20,
        CaptureStale = 0x00,
        CaptureFresh = 0x20,
        CaptureChannel = 0x40,
        CaptureRight = 0x00,
        CaptureLeft = 0x40,
        CaptureByte = 0x80,
        CaptureLower = 0x00,
        CaptureUpper = 0x80
    };

    namespace Shift
    {
        enum
        {
            InterruptStatus = 0x00,
            PlaybackDataStatus = 0x01,
            PlaybackChannel = 0x02,
            PlaybackByte = 0x03,
            SampleStatus = 0x04,
            CaptureStatus = 0x05,
            CaptureChannel = 0x06,
            CaptureByte = 0x07
        };
    }

    inline Status_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::Status);
    }

    inline void Write(Register_t baseAddress, Status_t value)
    {
        SYS_WritePortByte(baseAddress + Register::Status, value);
    }

}
}