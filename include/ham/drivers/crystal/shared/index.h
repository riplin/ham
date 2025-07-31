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
        Index = 0x00
    };
}

// Index Register

// +------+-----+-----+-----+-----+-----+-----+-----+
// |  D7  |  D6 |  D5 |  D4 |  D3 |  D2 |  D1 |  DO |
// +------+-----+-----+-----+-----+-----+-----+-----+
// | INIT | MCE | TRD | res | IA3 | IA2 | IA1 | IA0 |
// +------+-----+-----+-----+-----+-----+-----+-----+

// IA3-IAO     Index Address: These bits define the
//             address of the CS4248 register ac-
//             cessed by the Indexed Data Register.
//             These bits are read/write.

// res         Reserved for future expansion. Always
//             write zero to this bit.

// TRD         Transfer Request Disable: This bit,
//             when set, causes DMA transfers to
//             cease when the INT bit of the status
//             register is set.
//             0 - Transfers Enabled (PDRQ and
//                 CDRQ occur uninhibited)
//             1 - Transfers Disabled (PDRQ and
//                 CDRQ only occur if INT bit is 0)

// MCE         Mode Change Enable: This bit must be
//             set whenever the current mode of the
//             CS4248 is changed. The Data Format
//             and Interface Configuration registers
//             CANNOT be changed unless this bit is
//             set. The exceptions are CEN and PEN
//             which can be changed "on-the-fly"
//             No audo activity will occur when this
//             bit is set.

// INIT        CS4248 initialization: This bit is read as
//             1 when the CS4248 is in a state which
//             it cannot respond to parallel interface
//             cycles. This bit is read-only.

// Immediately after RESET (and once the CS4248
// has left the INIT state), the state of this register is:
// 010X0000 (40h)

// During CS4248 initialization, this register CAN-
// NOT be written and is always read
// 10000000 (80h)



typedef uint8_t Index_t;
namespace Index
{
    enum
    {
        Address = 0x0F,
        TransferRequest = 0x20,
        TransferRequestEnable = 0x00,
        TransferRequestDisable = 0x20,
        ModeChange = 0x40,
        ModeChangeDisable = 0x00,
        ModechangeEnable = 0x40,
        Initialize = 0x80
    };

    namespace Shift
    {
        enum
        {
            Address = 0x00,
            TransferRequest = 0x05,
            ModeChange = 0x06,
            Initialize = 0x07
        };
    }

    inline Index_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::Index);
    }

    inline void Write(Register_t baseAddress, Index_t value)
    {
        SYS_WritePortByte(baseAddress + Register::Index, value);
    }

}
}