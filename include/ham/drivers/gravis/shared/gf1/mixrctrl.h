//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <ham/drivers/gravis/shared/regtype.h>

namespace Ham::Gravis::Shared::GF1
{

namespace Register
{
    enum
    {
        MixerControl = 0x506
    };
}



typedef uint8_t MixerControl_t;
namespace MixerControl
{

    enum
    {

    };

    namespace Shift
    {
        enum
        {

        };
    }

    inline void Write(Register_t baseAddress, MixerControl_t value)
    {
        SYS_WritePortByte(baseAddress + Register::MixerControl, value);
    }

}

}
