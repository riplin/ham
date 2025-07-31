//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/tstinit.h>

namespace Ham::Crystal::Shared::IO
{

namespace TestAndInitialization
{

    inline TestAndInitialization_t Read(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::TestAndInitialization);
    }

    inline void Write(Register_t baseAddress, TestAndInitialization_t value)
    {
        Data::Write(baseAddress, Register::TestAndInitialization, value);
    }

}

}