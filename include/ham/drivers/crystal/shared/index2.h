//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/index.h>

namespace Ham::Crystal::Shared
{

// Index Register

// +------+-----+-----+-----+-----+-----+-----+-----+
// |  D7  |  D6 |  D5 |  D4 |  D3 |  D2 |  D1 |  DO |
// +------+-----+-----+-----+-----+-----+-----+-----+
// | INIT | MCE | TRD | IA4 | IA3 | IA2 | IA1 | IA0 |
// +------+-----+-----+-----+-----+-----+-----+-----+

// IA3-IAO     Index Address: These bits define the
//             address of the CS4248 register ac-
//             cessed by the Indexed Data Register.
//             These bits are read/write.

// IA4         Allows access to indirect registers 16
//             - 31. Only available in MODE 2. In
//             MODE 1, this bit is reserved.

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
//             CS4231 is changed. The Data Format
//             and Interface Configuration registers
//             CANNOT be changed unless this bit is
//             set. The exceptions are CEN and PEN
//             which can be changed "on-the-fly"
//             No audo activity will occur when this
//             bit is set.

// INIT        CS4231 initialization: This bit is read as
//             1 when the CS4231 is in a state which
//             it cannot respond to parallel interface
//             cycles. This bit is read-only.

// Immediately after RESET (and once the CS4231
// has left the INIT state), the state of this register is:
// 010X0000 (40h)

// During CS4231 initialization, this register CAN-
// NOT be written and is always read
// 10000000 (80h)

typedef uint8_t Index2_t;
namespace Index2
{
    enum
    {
        Address = Index::Address | 0x10,
        TransferRequest = Index::TransferRequest,
        TransferRequestEnable = Index::TransferRequestEnable,
        TransferRequestDisable = Index::TransferRequestDisable,
        ModeChange = Index::ModeChange,
        ModeChangeDisable = Index::ModeChangeDisable,
        ModechangeEnable = Index::ModechangeEnable,
        Initialize = Index::Initialize
    };

    namespace Shift
    {
        enum
        {
            Address = Index::Shift::Address,
            TransferRequest = Index::Shift::TransferRequest,
            ModeChange = Index::Shift::ModeChange,
            Initialize = Index::Shift::Initialize
        };
    }

    inline Index2_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::Index);
    }

    inline void Write(Register_t baseAddress, Index2_t value)
    {
        SYS_WritePortByte(baseAddress + Register::Index, value);
    }

}
}