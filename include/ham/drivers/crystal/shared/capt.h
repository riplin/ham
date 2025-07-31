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
        Capture = 0x03
    };
}

// The PIO Data register is two registers mapped to
// the same address. Writes to this register sends
// data to the Playback Data register. Reads from
// this register will receive data from the Capture
// Data register.

// The reading of this register will increment the
// state machine so that the following read will be
// from the next appropriate byte in the sample.
// The exact byte which is next to be read can be
// determined by reading the Status register. Once
// all relevant bytes have been read, the state ma-
// chine will point to the last byte of the sample
// until a new sample is received from the ADCs.
// Once this has occurred, and a read of the status
// has occurred, the state machine and status regis-
// ter will point to the first byte of the new sample.
// Until a new sample is received, reads from this
// register will return the most significant byte of
// the sample.

// During CS4248 initialization, this register CAN-
// NOT be written and is always read
// 10000000 (80h)

typedef uint8_t Capture_t;
namespace Capture
{

    inline Capture_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::Capture);
    }

}
}