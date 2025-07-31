//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        AlternateFeatureStatus = 0x18
    };
}

// Alternate Feature Status (I24)

// +------+-----+------+------+------+------+------+------+
// |  D7  | D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  DO  |
// +------+-----+------+------+------+------+------+------+
// | res  | TI  |  CI  |  PI  |  CU  |  CO  |  PO  |  PU  |
// +------+-----+------+------+------+------+------+------+

// PU  Playback Underrun: This bit, when set,
//     indicates that the DAC has run out
//     of data and a sample has been
//     missed.

// PO  Playback Overrun: This bit, when set,
//     indicates that the host attempted to
//     write data into a full FIFO and the
//     data was discarded.

// CO  Capture Overrun: This bit, when set,
//     indicates that the ADC had a sample
//     to load into the FIFO but the FIFO
//     was full. In this case the bit is set
//     and the new sample is discarded.

// CU  Capture Underrun: This bit indicates
//     that the host has read more data out
//     of the FIFO than it contained. In this
//     condition, the bit is set and the last
//     valid byte is read by the host.

// PI  Playback Interrupt: This bit indicates
//     that an interrupt is pending from the
//     playback DMA count registers.

// CI  Capture Interrupt: This bit indicates
//     that an interrupt is pending from the
//     record DMA count registers.

// TI  Timer Interrupt: This bit indicates that
//     an interrupt is pending from the
//     timer count registers

// The PI, Cl, and TI bits are reset by writing a "O" to
// the particular interrupt bit or by writing any value 1
// the Status register (R2).

// This register's initial state after reset is: x0000000

typedef uint8_t AlternateFeatureStatus_t;
namespace AlternateFeatureStatus
{

    enum
    {
        PlaybackStatus = 0x03,
        PlaybackHealthy = 0x00,
        PlaybackUnderrun = 0x01,
        PlaybackOverrun = 0x02,
        CaptureStatus = 0x0C,
        CaptureHealthy = 0x00,
        CaptureOverrun = 0x04,
        CaptureUnderrun = 0x08,
        PlaybackInterruptStatus = 0x10,
        PlaybackInterruptClear = 0x00,
        PlaybackInterruptPending = 0x10,
        CaptureInterruptStatus = 0x20,
        CaptureInterruptClear = 0x00,
        CaptureInterruptPending = 0x20,
        TimerInterruptStatus = 0x40,
        TimerInterruptClear = 0x00,
        TimerInterruptPending = 0x40
    };

    namespace Shift
    {
        enum
        {
            PlaybackStatus = 0x03,
            CaptureStatus = 0x0C,
            PlaybackInterruptStatus = 0x10,
            CaptureInterruptStatus = 0x20,
            TimerInterruptStatus = 0x40
        };
    };

}

}