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
        VolumeRampStart = 0x07,
        VolumeRampEnd = 0x08
    };
}

// Start:
// Bits 7-4    Exponent
// Bits 3-0    Mantissa
// This register specifies the starting position of a volume ramp. See the special section on volume ramping for a more
// complete explanation of how this register works.

// Please see section 2.22 for more information on hardware volume ramping.

// End:
// Bits 7-4    Exponent
// Bits 3-0    Mantissa
// This register specifies the ending position of a volume ramp. See the special section on volume ramping for a more
// complete explanation of how this register works.

// Note: The starting volume must always be less than the ending volume. If you want the volume
// to ramp down, turn on the decreasing volume bit in the Volume Control Register.

// 2.22. Volume ramping description
// The UltraSound has built-in volume ramping to facilitate the implementation of the attack decay sustain release
// envelopes. This ramping uses the same principle as the voices for updating the volume values. A section of the
// envelope can be programmed such that the PC does not need to be burdened with the task of changing each volume
// at specified intervals. At the end of that particular section, an IRQ can be generated so that the next section can be
// programmed in.
// This continues until the entire envelope has been completed. The start and end points as well as the increment rate
// are fully programmable.

// The hardware volume registers and bit definitions are:

// Current Volume (9,89) EEEEMMMMMMMM (Bits 15-4)

// Volume Start (7,87) EEEEMMMM (Bits 7-0)

// Volume End (8,88) EEEEMMMM (Bits 7-0)

// Volume Incr (6,86) RRMMMMMM (Bits 7-0)

// Once the current volume, start and end volumes are programmed, the only thing left is to set up the volume
// increment register. This register determines how fast the ramp takes place and with what granularity. The finer the
// granularity, the smoother (but slower) the ramp. The increment register has 2 fields. The first is the amount added
// to (or subtracted from) the current volume to get to the next one. These are the low 6 bits and can range from 1 to
// 63. A 1 is a long, slow ramp compared to a 63. The upper 2 bits determine how often the increment is applied to the
// current volume. The rate bits are defined as:

// Rate Bits       Volume Update Rate
// 00              FUR (FUR = 1/(1.6*#active voices))
// 01              FUR/8
// 10              FUR/64
// 11              FUR/512

// Each rate increment is 8 times longer than the preceding one. This means that the value to store for the fastest
// possible ramp is 1Fh (63), and the value for the slowest possible ramp is 1Ch (193). The approximate times for a
// full scale volume ramp (0-4095) are:

// Rate Vol Inc    14 Voices 32 Voices
// ---- -------    --------- ---------
// 0       63         1.4 ms    3.3 ms
// 0        1        91.7 ms  209.7 ms
// 1       63        11.5 ms   26.2 ms
// 1        1       733.8 ms   1.7 sec
// 2       63        91.8 ms  209.7 ms
// 3        1        5.9 sec  13.4 sec
// 3       63       734.0 ms   1.7 sec
// 3        1       47.0 sec 107.3 sec

// Note that these times are for full sweep ramping. Since a volume ramp usually goes between points in between the
// limits, the actual ramp times will be much smaller.

// Allowing to let the volume ramps to go to the extremes can cause a random oscillation of the volume when it reaches
// the limits. This is caused by an overshoot past the limit due to a large step size. The SDK routines protect against
// this by limiting how close to the rails you can get.


typedef Indexed::Register8_t VolumeRamp_t;
namespace VolumeRamp
{

    enum
    {
        Mantissa = 0x0f,
        Exponent = 0xf0
    };

    namespace Shift
    {
        enum
        {
            Mantissa = 0x00,
            Exponent = 0x04
        };
    }

    inline VolumeRamp_t ReadStart(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::VolumeRampStart) + uint8_t(Register::BaseRead));
    }

    inline VolumeRamp_t ReadEnd(Register_t baseAddress)
    {
        return Data::Read8High(baseAddress, uint8_t(Register::VolumeRampEnd) + uint8_t(Register::BaseRead));
    }

    inline void WriteStart(Register_t baseAddress, VolumeRamp_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::VolumeRampStart) + uint8_t(Register::BaseWrite), value);
    }

    inline void WriteEnd(Register_t baseAddress, VolumeRamp_t value)
    {
        Data::Write8High(baseAddress, uint8_t(Register::VolumeRampEnd) + uint8_t(Register::BaseWrite), value);
    }

}

}