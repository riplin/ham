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
        Page = 0x102
    };
}

// This could also be called the voice select register. This register is used to specify which voice's registers you want to
// read/write. This value can range from 0 to the number of active voices specified (13-31). Once this has been
// specified, you may select the specific register within that voice. Be careful that IRQs are off during the time that the
// page and select registers are being modified. This will prevent the foreground from selecting a voice and having the
// background change it in the background.

typedef uint8_t Page_t;
namespace Page
{

    inline Page_t Read(Register_t baseAddress)
    {
        return SYS_ReadPortByte(baseAddress + Register::Page);
    }

    inline void Write(Register_t baseAddress, Page_t value)
    {
        SYS_WritePortByte(baseAddress + Register::Page, value);
    }

}

}
