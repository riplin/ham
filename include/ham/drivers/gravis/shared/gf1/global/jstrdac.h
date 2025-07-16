//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/gf1/data.h>

namespace Ham::Gravis::Shared::GF1::Global
{

namespace Register
{
    enum
    {
        JoyStickTrimDac = 0x4b
    };
}

// This register is initialized to 4.3 volts (value = 29). It only needs to be modified to account for faster/slower
// machines. A utility is provided (ULTRAJOY.EXE) that sets this up. There should be no reason for your application
// to modify this register.

typedef Indexed::Register8_t JoyStickTrimDac_t;
namespace JoyStickTrimDac
{

    enum
    {
        Value = 0xff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    inline void Write(Register_t baseAddress, JoyStickTrimDac_t value)
    {
        Data::Write8High(baseAddress, Register::JoyStickTrimDac, value);
    }

}

}
