//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/afeii.h>

namespace Ham::Crystal::Shared::IO
{

namespace AlternateFeatureEnableII
{

    inline AlternateFeatureEnableII_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::AlternateFeatureEnableII);
    }

    inline void Write(Register_t baseAddress, AlternateFeatureEnableII_t value)
    {
        Data::Write(baseAddress, Register::AlternateFeatureEnableII, value);
    }

}

}