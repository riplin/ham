//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/amd/shared/codec/regtype.h>

namespace Ham::Amd::Shared::Codec
{

namespace Register
{
    enum
    {
        PlaybackVariableFrequency = 0x1D
    };
}

// CPVFI—Playback Variable Frequency

// Address: PCODAR+1 read, write; index CIDXR[4:0]=1Dh

// Default: 00h

// Modes: 3 only

// This 8-bit register specifies the playback frequency when the Variable Frequency Playback
// bit of the Configuration Register 3 (CFIG3I[2]) is set High. The playback frequency is
// PCS/(16•(48+CPVFI)), where PCS is the frequency of the oscillator selected in the Playback
// Crystal Select bit of the Playback Data Format register (CPDFI[0]). The 16.9-MHz oscillator
// provides a range from about 3.5 kHz to 22.05 kHz; the 24.5-MHz oscillator provides a range
// from about 5.0 kHz to 32 kHz. It is not necessary to set the Mode Change Enable bit of the
// Codec Index Address register (CIDXR[6]) High when altering the value of this register.
// Note: It is not recommended that the Playback Crystal Select field of the Playback
// Data Format register (CPDFI[0]) be changed while in variable frequency playback
// mode. Doing so causes glitches in internal circuitry and can produce unpredictable
// results.

typedef Crystal::CS4231::Data_t PlaybackVariableFrequency_t;

namespace IO::PlaybackVariableFrequency
{
    inline PlaybackVariableFrequency_t Read(Register_t baseAddress)
    {
        return Crystal::CS4231::Data::Read(baseAddress, Register::PlaybackVariableFrequency);
    }

    inline void Write(Register_t baseAddress, PlaybackVariableFrequency_t value)
    {
        Crystal::CS4231::Data::Write(baseAddress, Register::PlaybackVariableFrequency, value);
    }
}

}