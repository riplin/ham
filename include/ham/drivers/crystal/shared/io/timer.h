//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/regs/timer.h>

namespace Ham::Crystal::Shared::IO
{

namespace Timer
{
    
    inline Timer8_t ReadHigh(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::TimerHighByte);
    }

    inline Timer8_t ReadLow(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Register::TimerLowByte);
    }

    inline Timer16_t Read(Register_t baseAddress)
    {
        return (Timer16_t(ReadHigh(baseAddress)) << 8) | ReadLow(baseAddress);
    }

    inline void WriteHigh(Register_t baseAddress, Timer8_t value)
    {
        Data::Write(baseAddress, Register::TimerHighByte, value);
    }

    inline void WriteLow(Register_t baseAddress, Timer8_t value)
    {
        Data::Write(baseAddress, Register::TimerLowByte, value);
    }

    inline void Write(Register_t baseAddress, Timer16_t value)
    {
        WriteHigh(baseAddress, value >> 16);
        WriteLow(baseAddress, Timer8_t(value));
    }

}

}