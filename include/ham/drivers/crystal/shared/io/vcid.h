//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/vcid.h>

namespace Ham::Crystal::Shared::IO
{

namespace VersionAndChipId
{

    inline VersionAndChipId_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::VersionAndChipId);
    }

    inline void Write(Register_t baseAddress, VersionAndChipId_t value)
    {
        Data::Write(baseAddress, Register::VersionAndChipId, value);
    }

}

}