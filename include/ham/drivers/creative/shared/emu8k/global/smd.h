//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/creative/shared/emu8k/data.h>

namespace Ham::Creative::Shared::Emu8k::Global
{

namespace Register
{
    enum
    {
        SoundMemoryLeftData = 0x01,
        SoundMemoryRightData = 0x01
    };
}

// This word register is used for transfering data to and from sound memory using the “left” DMA
// stream. If one or more channels are programmed for DMA write using the left stream, a write of
// a word of data to this register will cause that data to be written to the address currently in
// SMALW. SMALW will be updated after the data is written. SMALW’s “full” bit will be set
// from the time the data is written in SMLD until it is transfered to sound memory. If a second
// write is attempted to SMLD while it is full, the I/O WAIT mechanism will hold off transfer until
// either the data is written or it is aborted because no channels are currently programmed for
// writing the left DMA stream.

// If one or more channels are programmed for DMA read using the left stream, a read of a word of
// data from this register will transfer the current “read” contents of SMLD, and cause SMALR’s
// “empty” bit to be set until data can be transfered from sound memory to the SMLD read
// register. Data will be read from the address currently in SMALR, and SMALR will be updated
// after the data is read from sound memory. If a second read is attempted from SMLD while it is
// empty, the I/O WAIT mechanism will hold off transfer until either the data is read from sound
// memory or it is aborted because no channels are currently programmed for reading the left DMA
// stream.

// Note that SMLD really consists of separate read and write registers, based on fully independent
// DMA streams. Note also that the read function involves a pre-fetch of data. This implies that
// under normal operation, one must read a word of “stale” data from the SMLD register before
// initiating a read transfer.

typedef uint32_t SoundMemoryData_t;
namespace SoundMemoryData
{

    inline SoundMemoryData_t ReadLeft(Register_t baseAddress)
    {
        return Data::Read1Word(baseAddress, Register::SoundMemoryLeftData, 26);
    }

    inline SoundMemoryData_t ReadRight(Register_t baseAddress)
    {
        return Data::Read2Word(baseAddress, Register::SoundMemoryRightData, 26);
    }

    inline void WriteLeft(Register_t baseAddress, SoundMemoryData_t value)
    {
        Data::Write1Word(baseAddress, Register::SoundMemoryLeftData, 26, value);
    }

    inline void WriteRight(Register_t baseAddress, SoundMemoryData_t value)
    {
        Data::Write2Word(baseAddress, Register::SoundMemoryRightData, 26, value);
    }

}

}