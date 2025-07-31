//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/modeid.h>

namespace Ham::Crystal::Shared::IO
{

namespace ModeAndId
{

    inline ModeAndId_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::ModeAndId);
    }

    inline void Write(Register_t baseAddress, ModeAndId_t value)
    {
        Data::Write(baseAddress, Register::ModeAndId, value);
    }

}

}