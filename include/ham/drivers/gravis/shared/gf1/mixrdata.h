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
        MixerData = 0x106
    };
}



typedef uint8_t MixerData_t;
namespace MixerData
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

    inline void Write(Register_t baseAddress, MixerData_t value)
    {
        SYS_WritePortByte(baseAddress + Register::MixerData, value);
    }

}

}
