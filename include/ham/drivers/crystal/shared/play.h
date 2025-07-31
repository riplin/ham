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
        Play = 0x03
    };
}

// Writing data to this register will increment the
// playback byte tracking state machine so that the
// following write will be to the correct byte of the
// sample. Once all bytes of a sample have been
// written, subsequent byte writes to this port are
// ignored. The state machine is reset when the cur-
// rent sample is sent to the DACs.

typedef uint8_t Play_t;
namespace Play
{

    inline void Write(Register_t baseAddress, Play_t value)
    {
        SYS_WritePortByte(baseAddress + Register::Play, value);
    }

}
}