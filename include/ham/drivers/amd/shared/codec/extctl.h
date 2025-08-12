//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/amd/shared/codec/regtype.h>

namespace Ham::Amd::Shared::Codec
{

// CEXTI—External Control

// Address: PCODAR+1 read, write; index CIDXR[4:0]=Ah

// Default: 00XX 0X0X

// Modes: 1, 2, and 3

// This register contains the global interrupt enable control as well as control bits for the two
// general purpose external output pins.

// +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------+
// | Bit 7           Bit 6          | Bit 5          | Bit 4          | Bit 3          | Bit 2          | Bit 1          | Bit 0    |
// +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------+
// | General Purpose Output Flags   | Reserved       | Reserved       | Read/Write Bit | Reserved       | Global         | Reserved |
// |                                |                |                |                |                | Interrupt      |          |
// |                                |                |                |                |                | Enable         |          |
// +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------+

// Bits 7–6:   General Purpose Output Flags. The state of these bits are reflected on
//             the GPOUT1 and GPOUT0 pins. These two pins default to IRQ10 and IRQ4.
//             They become the general purpose output flags when the Select GPOUT
//             Codec Flags bit of the Emulation IRQ register (IEIRQI[7]) is High.

// Bits 5–4:   Reserved.

// Bit 3:      Read/Write Bit. This bit can be read and written but it does not control
//             anything within the InterWave IC.

// Bit 2:      Reserved.

// Bit 1:      Global Interrupt Enable. If set High, enables codec interrupts. If set Low,
//             codec interrupts are not passed on to the selected IRQ pin. The status bits
//             are not affected by the state of this bit.

// Bit 0:      Reserved.

typedef Crystal::CS4231::PinControl_t ExternalControl_t;
namespace ExternalControl
{
    enum
    {
        InterruptControl = Crystal::CS4231::PinControl::InterruptControl,
        InterruptDisable = Crystal::CS4231::PinControl::InterruptDisable,
        InterruptEnable = Crystal::CS4231::PinControl::InterruptEnable,
        GeneralPurposeOut0 = Crystal::CS4231::PinControl::Crystal0Control,
        GeneralPurposeOut0Low = Crystal::CS4231::PinControl::Crystal0TtlLow,
        GeneralPurposeOut0High = Crystal::CS4231::PinControl::Crystal0TtlHigh,
        GeneralPurposeOut1 = Crystal::CS4231::PinControl::Crystal1Control,
        GeneralPurposeOut1Low = Crystal::CS4231::PinControl::Crystal1TtlLow,
        GeneralPurposeOut1High = Crystal::CS4231::PinControl::Crystal1TtlHigh
    };
}

namespace IO::ExternalControl
{
    inline ExternalControl_t Read(Register_t baseAddress)
    {
        return Crystal::CS4231::IO::PinControl::Read(baseAddress);
    }
    
    inline void Write(Register_t baseAddress, ExternalControl_t value)
    {
        Crystal::CS4231::IO::PinControl::Write(baseAddress, value);
    }
}

}