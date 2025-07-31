//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/afs.h>

namespace Ham::Crystal::Shared::IO
{

namespace AlternateFeatureStatus
{

    inline AlternateFeatureStatus_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::AlternateFeatureStatus);
    }

    inline void Write(Register_t baseAddress, AlternateFeatureStatus_t value)
    {
        Data::Write(baseAddress, Register::AlternateFeatureStatus, value);
    }

}

}