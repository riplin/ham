//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/amd/shared/codec/regtype.h>

namespace Ham::Amd::Shared::Codec
{

// CFIG3I—Configuration Register 3

// Address:    PCODAR+1 read, write; index CIDXR[4:0]=11h

// Default: 0000 X000

// Modes:  bits 7–1 in mode 3; bit 0 in modes 2 and 3

// In mode 3, this register provides for the programming of FIFO thresholds and the generation
// of I/O-mode FIFO service interrupts.

// +------------------+------------------+----------------+----------------+----------------+----------------+----------------+------------+
// | Bit 7            | Bit 6            | Bit 5          | Bit 4          | Bit 3          | Bit 2          | Bit 1          | Bit 0      |
// +------------------+------------------+----------------+----------------+----------------+----------------+----------------+------------+
// | Record FIFO      | Playback FIFO    | FIFO Threshold Select           | ADPCM          | Variable       | Synth Signal   | reserved   |
// | Service Request  | Service Request  |                                 | Record         | Frequency      | Select         |            |
// | Interrupt Enable | Interrupt Enable |                                 | Suspend        | Playback Mode  |                |            |
// +------------------+------------------+----------------+----------------+----------------+----------------+----------------+------------+

// Bit 7:      Record FIFO Service Request Interrupt Enable. When the record path is
//             enabled—the Record Enable bit of the Configuration Register 1 (CFIG1I[1])
//             is set High—setting this bit High enables the generation of an interrupt
//             request when the Record FIFO Interrupt Request bit in the Codec Status
//             Register 3 (CSR3I) goes High. This bit can be accessed in mode 3 only.

// Bit 6:      Playback FIFO Service Request Interrupt Enable. When the playback
//             path is enabled—the Playback Enable bit of the Configuration Register 1
//             (CFIG1I[0]) is set High—setting this bit High enables the generation of an
//             interrupt request when the Playback FIFO Interrupt Request bit in Codec
//             Status Register 3 (CSR3I) goes High. This bit can be accessed in mode 3
//             only.

// Bits 5–4:   FIFO Threshold Select. These two bits specify the record and playback
//             FIFO thresholds, as listed in Table 13-2.
//             When the appropriate bit of the Configuration Register 1 is Low—Record
//             FIFO I/O Select (CFIG1I[7]) or Playback FIFO I/O Select (CFIG1I[6])—a
//             DMA request occurs when the FIFO threshold is reached. The DMA
//             transfers occur until the playback FIFO is full or the record FIFO is empty.
//             The appropriate interrupt bit in the Codec Status Register 3 (CSR3I[5] for
//             record, CSR3I[4] for playback) is set High when the FIFO threshold is
//             reached and the following conditions exist:
//                 • The FIFO I/O select bit is High
//                 • The InterWave IC is programmed for mode 3 operation
//                 • The appropriate FIFO service request interrupt enable bit is High
//                   (CFIG3I[7] for record, CFIG3I[6] for playback).

// Table 13-2
// FIFO Threshold Selections
// +--------+---------------------------------------------------------+
// | FT 1 0 | Point at Which Request Becomes Active                   |
// +--------+---------------------------------------------------------+
// | 0 0    | Minimum: Record FIFO not empty; playback FIFO not full  |
// | 0 1    | Middle: Record FIFO half full; playback FIFO half empty |
// | 1 0    | Maximum: Record FIFO full; playback FIFO empty          |
// | 1 1    | Reserved                                                |
// +--------+---------------------------------------------------------+

// These bits are active in mode 3 only and have no effect in modes 1 and 2.

// For a complete understanding of the FIFO threshold interrupt structure, see
// “Codec Interrupt Structure” on page 6-6.


// Bit 3:      ADPCM Record Suspend. When the record path is in ADPCM mode,
//             setting this bit High causes recording to suspend. Setting this bit Low allows
//             recording to resume without resetting the ADPCM index that is used when
//             calculating ADPCM values. The index should be reset only at the beginning
//             of a file of record data. The index is reset when the Record Enable bit of the
//             Configuration Register 1 (CFIG1I[1]) is inactive (Low).

// Bit 2:      Variable Frequency Playback Mode. If set High, selects
//             variable-frequency-playback mode. In this mode, the sample rate is selected
//             by a combination of the Playback Crystal Select bit of the Playback Data
//             Format register (CPDFI[0]) and the Playback Variable Frequency register
//             (CPVFI) to allow variable frequencies between 3.5 kHz and 32 kHz. This bit
//             can be accessed in mode 3 only.

// Bit 1:      Aux 1/Synth Signal Select. This bit selects the source of the signals that
//             enter the Left/Right Auxiliary 1/Synthesizer Input Control register (CLAX1I
//             and CRAX1I) attenuators before entering the left and right mixers. This bit
//             Low selects the AUX1[L,R] input pins. If set High, selects the output of the
//             synthesizer DACs. This bit can be accessed in mode 3 only.

// Bit 0:      Read/Write Bit. This bit can be read and written but it does not control
//             anything within the InterWave IC. This bit can be accessed in mode 2 and
//             mode 3.


typedef Crystal::CS4231::AlternateFeatureEnableII_t Configuration3_t;
namespace Configuration3
{
    enum
    {
        Aux1SynthControl = 0x02,
        Aux1SyntSelectAux1 = 0x00,
        Aux1SynthSelectSynth = 0x02,
        PlaybackFrequencyControl = 0x04,
        PlaybackFrequencyFixed = 0x00,
        PlaybackFrequencyVariable = 0x04,
        AdpcmRecordControl = 0x08,
        AdpcmRecordResume = 0x00,
        AdpcmRecordSuspend = 0x08,
        FifoThresholdControl = 0x30,
        FifoThresholdMinimum = 0x00,
        FifoThresholdMiddle = 0x10,
        FifoThresholdMaximum = 0x20,
        PlaybackFifoInterruptControl = 0x40,
        PlaybackFifoInterruptDisable = 0x00,
        PlaybackFifoInterruptEnable = 0x40,
        RecordFifoInterruptControl = 0x80,
        RecordFifoInterruptDisable = 0x00,
        RecordFifoInterruptEnable = 0x80
    };
}

namespace IO::Configuration3
{
    inline Configuration3_t Read(Register_t baseAddress)
    {
        return Crystal::CS4231::IO::AlternateFeatureEnableII::Read(baseAddress);
    }
    
    inline void Write(Register_t baseAddress, Configuration3_t value)
    {
        Crystal::CS4231::IO::AlternateFeatureEnableII::Write(baseAddress, value);
    }
}

}