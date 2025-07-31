//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/misc.h>

namespace Ham::Crystal::Shared::IO
{

namespace Miscellaneous
{

    inline Miscellaneous_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::Miscellaneous);
    }

    inline void Write(Register_t baseAddress, Miscellaneous_t value)
    {
        Data::Write(baseAddress, Register::Miscellaneous, value);
    }

}

}