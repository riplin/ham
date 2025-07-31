//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/afei.h>

namespace Ham::Crystal::Shared::IO
{

namespace AlternateFeatureEnableI
{

    inline AlternateFeatureEnableI_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::AlternateFeatureEnableI);
    }

    inline void Write(Register_t baseAddress, AlternateFeatureEnableI_t value)
    {
        Data::Write(baseAddress, Register::AlternateFeatureEnableI, value);
    }

}

}