//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/data.h>
#include <ham/drivers/gravis/shared/gf1/voice/vocbase.h>

namespace Ham::Gravis::Shared::GF1::Voice
{

namespace Register
{
    enum
    {
        VolumeRampControl = 0x0d
    };
}

// =================================
// | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// =================================
//   |   |   |   |   |   |   |   |
//   |   |   |   |   |   |   |   +---- Ramp Stopped
//   |   |   |   |   |   |   +-------- Stop Ramp
//   |   |   |   |   |   +------------ Rollover condition
//   |   |   |   |   +---------------- Loop Enable
//   |   |   |   +-------------------- Bi-directional loop enable
//   |   |   +------------------------ Volume ramp IRQ enable
//   |   +---------------------------- Direction
//   +-------------------------------- IRQ pending

// * Bit 0     - Show the ramp has stopped
//   Bit 1     - Manually stop the ramp.
//   Bit 2     - Roll over condition. This bit pertains more towards the location of the voice rather than
//               its volume. Its purpose is to generate an IRQ and NOT stop (or loop). It will generate an IRQ and
//               the voice's address will continue to move through DRAM in the same direction. This can be a
//               very powerful feature. It allows the application to get an interrupt without having the sound stop.
//               This can be easily used to implement a ping-pong buffer algorithm so an application can keep
//               feeding it data and there will be no pops. Even if looping is enabled, it will not loop.
//   Bit 3     - Enable looping. Loop from end to start (or start to end).
//   Bit 4     - Enable bi-directional looping. When it hits end (or start) it will change directions and
//               proceed toward the other limit.
//   Bit 5     - Enable getting an IRQ when ramp hits end.
// * Bit 6     - Ramp direction. 0=increasing, 1=decreasing.
// * Bit 7     - (READ) Volume ramp IRQ pending.

// Please see section 2.22 for more information on hardware volume ramping.

typedef Indexed::Register8_t VolumeRampControl_t;
namespace VolumeRampControl
{

    enum
    {
        Status = 0x01,  // *
        Running = 0x00, // *
        Stopped = 0x01, // *

        Control = 0x02,
        Run = 0x00,
        Stop = 0x02,

        RollOverCondition = 0x04,

        Looping = 0x08,
        LoopDisable = 0x00,
        LoopEnable = 0x08,

        Directionality = 0x10,
        Unidirectional = 0x00,
        Bidirectional = 0x10,

        InterruptControl = 0x20,
        InterruptDisable = 0x00,
        InterruptEnable = 0x20,

        Direction = 0x40,   // *
        Increasing = 0x00,  // *
        Decreasing = 0x40,  // *

        //Read:
        InterruptStatus = 0x80, // *
        InterruptClear = 0x00,  // *
        InterruptPending = 0x80 // *
    };

    namespace Shift
    {
        enum
        {
            Status = 0x00,
            Control = 0x01,
            RollOverCondition = 0x02,
            Looping = 0x03,
            Directionality = 0x04,
            InterruptControl = 0x05,
            Direction = 0x06,
            InterruptStatus = 0x07
        };
    }

    inline VolumeRampControl_t Read(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::VolumeRampControl) + uint8_t(Register::BaseRead));
    }

    inline void Write(Register_t baseAddress, VolumeRampControl_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::VolumeRampControl) + uint8_t(Register::BaseWrite), value);
    }

}

}