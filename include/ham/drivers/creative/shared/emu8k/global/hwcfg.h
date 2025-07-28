//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Global
{

namespace Register
{
    enum
    {
        Configuration1 = 0x01,
        Configuration2 = 0x01,
        Configuration3 = 0x01,
        Configuration4 = 0x01,
        Configuration5 = 0x01,
        Configuration6 = 0x01,
    };
}

// Register : HWCF1
// Description : Configuration Word 1
// Value 0x0059 should be written to this word register after power-up. After this, it can be
// ignored. Note: Due to a VLSI error, this register will not be correctly read by the processor.

// Register : HWCF2
// Description : Configuration Word 2
// Value 0x0020 should be written to this word register after power-up. After this, it can be
// ignored. Note: Due to a VLSI error, this register will not be correctly read by the processor.

// Register : HWCF3
// Description : Configuration Word 3
// Value 0x0004 should be written to this word register after all other initialization is complete.
// After this, it can be ignored. Note: Due to a VLSI error, this register will not be correctly read
// by the processor.

// Register : HWCF4
// Description : Configuration DoubleWord 4
// Zero should be written to this doubleword register during the initialization process. After this, it
// can be ignored.

// Register : HWCF5
// Description : Configuration DoubleWord 5
// Value 0x00000083 should be written to this doubleword register during the initialization process.
// After this, it can be ignored.

// Register : HWCF6
// Description : Configuration DoubleWord 6
// Value 0x00008000 should be written to this doubleword during the initialization process. After
// this, it can be ignored.

typedef uint32_t Configuration32_t;
typedef uint16_t Configuration16_t;
namespace Configuration
{

    inline Configuration16_t Read1(Register_t baseAddress)
    {
        return Data::Read1Word(baseAddress, Register::Configuration1, 29);
    }

    inline Configuration16_t Read2(Register_t baseAddress)
    {
        return Data::Read1Word(baseAddress, Register::Configuration2, 30);
    }

    inline Configuration16_t Read3(Register_t baseAddress)
    {
        return Data::Read1Word(baseAddress, Register::Configuration3, 31);
    }

    inline Configuration32_t Read4(Register_t baseAddress)
    {
        return Data::Read1DWord(baseAddress, Register::Configuration4, 9);
    }

    inline Configuration32_t Read5(Register_t baseAddress)
    {
        return Data::Read1DWord(baseAddress, Register::Configuration5, 10);
    }

    inline Configuration32_t Read6(Register_t baseAddress)
    {
        return Data::Read1DWord(baseAddress, Register::Configuration6, 13);
    }

    inline void Write1(Register_t baseAddress, Configuration16_t value)
    {
        Data::Write1Word(baseAddress, Register::Configuration1, 29, value);
    }

    inline void Write2(Register_t baseAddress, Configuration16_t value)
    {
        Data::Write1Word(baseAddress, Register::Configuration2, 30, value);
    }

    inline void Write3(Register_t baseAddress, Configuration16_t value)
    {
        Data::Write1Word(baseAddress, Register::Configuration3, 31, value);
    }

    inline void Write4(Register_t baseAddress, Configuration32_t value)
    {
        Data::Write1DWord(baseAddress, Register::Configuration4, 9, value);
    }

    inline void Write5(Register_t baseAddress, Configuration32_t value)
    {
        Data::Write1DWord(baseAddress, Register::Configuration5, 10, value);
    }

    inline void Write6(Register_t baseAddress, Configuration32_t value)
    {
        Data::Write1DWord(baseAddress, Register::Configuration6, 13, value);
    }

}

}