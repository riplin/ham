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
        RevisionLevel = 0x506
    };
}



typedef uint8_t RevisionLevel_t;
namespace RevisionLevel
{

    enum
    {
        Pre3p7 = 0xff,  // Pre 3.7 boards. ICS mixer NOT present
        Rev3p7 = 0x05,  // Rev 3.7 with ICS Mixer. Some R/L: flip problems.
        Rev3p7p = 0x06, // until 0x09 Revision 3.7 and above. ICS Mixer present
        Max = 0x0a // and up. UltraMax. CS4231 present, no ICS mixer
    };

    inline RevisionLevel_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::RevisionLevel);
    }

}

}
