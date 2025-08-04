//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/data.h>

namespace Ham::Amd::Shared::PnP
{

namespace Register
{
    enum
    {
        EepromEnable = 0xF0
    };
}

// Address: 0A79h write, PNPRDP read; index PIDXR=F0h and PLDNI=0

// Default: 00h

// This register is accessible only when the PNP state machine is in the configuration state.

// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+
// | Bit 7      | Bit 6          | Bit 5          | Bit 4          | Bit 3          | Bit 2          | Bit 1          | Bit 0        |
// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+
// | reserved   | reserved       | reserved       | reserved       | reserved       | reserved       | ISA Data Bus   | Serial       |
// |            |                |                |                |                |                | Drive          | EEPROM       |
// |            |                |                |                |                |                |                | Mode         |
// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+

// Bits 7–2:   Reserved.

// Bit 1:      ISA Data Bus Drive. This bit specifies the output-low drive capability, Iol,
//             of the SD15–SD0, IOCHRDY, IOCS16, and IOCHK pins. At 5 V: 00=24 mA,
//             01=12mA, 10=3mA, 11=reserved. At 3.3 V, the drive is at least 3 mA for bits
//             2–1 = 00, 01, and 10.

// Bit 0:      Serial EEPROM Mode. If set Low, specifies that the serial EEPROM
//             interface circuitry is in initialization mode, whereby the data transfer is
//             controlled by the PNP state machine. If set High, the serial EEPROM is
//             controlled directly by the PNP Serial EEPROM Control register (PSECI).

typedef Has::System::PnP::Data_t EepromEnable_t;
namespace EepromEnable
{
    enum
    {
        Mode = 0x01,
        IsaDataBusDrive = 0x02
    };

    inline EepromEnable_t Read()
    {
        return Has::System::PnP::Data::Read(Register::EepromEnable);
    }

    inline void Write(EepromEnable_t eepromEnable)
    {
        Has::System::PnP::Data::Write(Register::EepromEnable, eepromEnable);
    }

}

}