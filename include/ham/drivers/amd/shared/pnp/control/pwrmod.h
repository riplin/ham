//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/data.h>

namespace Ham::Amd::Shared::PnP
{

namespace Register
{
    enum
    {
        PowerMode = 0xF2
    };
}

// PPWRI—PNP Power Mode

// Address: 0A79h write, PNPRDP read; index PIDXR=F2h and PLDNI=0

// Default: X111 1111

// Use this register to enable and disable clocks and low-power modes for major sections of
// the InterWave IC. Writing to this register is accomplished differently from most. Bit 7
// specifies the value to be written (1 or 0); for bits 6–0, a High indicates that the value in bit
// 7 is to be written into the bit and a Low indicates that the bit is to be left unmodified. Thus,
// to modify a subset of bits 6–0, it is not necessary to read the register ahead of time to
// determine the state of bits that should not change. Examples: To set bit 0 High, write 81h;
// to clear bit 4, write 01h.

// If a single command comes to set bits 6–1 Low (I/O write of 0111 111X, binary), then the
// IC enters shut-down mode and the 16.9-MHz oscillator becomes disabled. When,
// subsequently, one or more of bits 6–1 are set High, the 16.9-MHz oscillator is re-enabled.
// After being re-enabled, the 16.9-MHz clock requires 4 ms to 8 ms to become stable.
// This register is accessible only when the IC is in the PNP configuration state.

// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+
// | Bit 7      | Bit 6          | Bit 5          | Bit 4          | Bit 3          | Bit 2          | Bit 1          | Bit 0        |
// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+
// | Enable     | 24.576-MHz     | Local Memory   | Synthesizer    | Game–MIDI      | Codec          | Codec Record   | Codec Analog |
// |            | Oscillator     | Control        | Enable         | Ports Enable   | Playback Path  | Path Enable    | Circuitry    |
// |            | Enable         | Enable         |                |                | Enable         |                | Enable       |
// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+

// Bit 7:      Enable. Specifies the value that is to be written to bits 6–0 of the register.
//             In all seven cases, a High specifies that the block is functional and a Low
//             indicates that it is in low-power mode.

// Bit 6:      24.576-MHz Oscillator Enable. If set Low, causes the 24.576-MHz
//             oscillator to stop. It is not recommended that this oscillator be disabled if
//             either the Playback Crystal Select bit of the Playback Data Format register
//             (CPDFI[0]) or the Record Crystal Select bit of the Record Data Format
//             register (CRDFI[0]) are Low. However, it is okay to set this bit Low as part
//             of the shut-down command, regardless of the state of CPDFI[0] and
//             CRDFI[0].

// Bit 5:      Local Memory Control Enable. If set Low, disables the 16.9-MHz clock to
//             the local memory control module and allows slow refresh cycles to local
//             DRAM using C32KHZ.

// Bit 4:      Synthesizer Enable. If set Low, disables the 16.9-MHz clock to the
//             synthesizer module and the clocks to the synthesizer DAC input to the codec
//             mixer.

// Bit 3:      Game–MIDI Ports Enable. If set Low, disables all clocks to the ports module
//             and disables internal and external resistors from consuming current.

// Bit 2:      Codec Playback Path Enable. If set Low, disables clocks to the codec
//             playback path including the playback FIFO, format conversion, filtering, and
//             DAC.

// Bit 1:      Codec Record Path Enable. If set Low, disables clocks to the codec record
//             path including the record FIFO, format conversion, filtering, and ADC.

// Bit 0:      Codec Analog Circuitry Enable. If set Low, disables all the codec analog
//             circuitry and places it in a low-power mode. If set Low, the following analog
//             pins are placed into the high-impedance state: MIC[L,R], AUX1[L,R],
//             AUX2[L,R], LINEIN[L,R], MONOIN, LINEOUT[L,R], MONOOUT, CFILT, IREF.


typedef Has::System::PnP::Data_t PowerMode_t;
namespace PowerMode
{
    enum
    {
        CodecAnalogCircuitry = 0x01,
        CodecRecordPath = 0x02,
        CodecPlaybackPath = 0x04,
        GameMidiPorts = 0x08,
        Synthesizer = 0x10,
        LocalMemoryControl = 0x20,
        Oscillator24p576 = 0x40,
        Control = 0x80,
        Disable = 0x00,
        Enable = 0x80
    };

    inline void Write(PowerMode_t powerMode)
    {
        Has::System::PnP::Data::Write(Register::PowerMode, powerMode);
    }
}

}