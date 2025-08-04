//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/data.h>

namespace Ham::Amd::Shared::PnP
{

namespace Register
{
    enum
    {
        EepromControl = 0xF1
    };
}

// PSECI—PNP Serial EEPROM Control

// Address: 0A79h write, PNPRDP read; index PIDXR=F1h and PLDNI=0

// Default: XXXX 000X

// If the InterWave IC is in control mode—the Serial EEPROM Mode bit of the PNP Serial
// EEPROM Enable register (PSEENI[0]) set High—and the PNP Audio Activate register
// (PUACTI[0]) is inactive, then bits 2–0 of this register are used to directly control the serial
// EEPROM. Bits 7–4 are read-only status bits that show the state of various control signals
// that are latched at the trailing edge of RESET. For details, see Appendix A, “Packaging
// and Pin Designations.” This register is accessible only when the PNP state machine is in
// the configuration state.

// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+
// | Bit 7      | Bit 6          | Bit 5          | Bit 4          | Bit 3          | Bit 2          | Bit 1          | Bit 0        |
// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+
// | SUSPEND-C  | External       | PNP System     | VCC is 5 V     | Serial         | Serial         | Serial         | Serial       |
// | 32KHz      | Decode         | Board Select   |                | EEPROM         | EEPROM         | EEPROM         | EEPROM       |
// | Select     | Select         |                |                | Chip Select    | Serial Clock   | Data In        | Data Out     |
// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+

// Bit 7:      SUSPEND-C32KHZ Select. Provides the state of the internal signal
//             IPSUS32, which is latched off the RA[21] pin at the trailing edge of RESET.

// Bit 6:      External Decode Select. Provides the state of the internal signal IPEXDEC
//             which is latched off the RA[20] pin at the trailing edge of RESET.

// Bit 5:      PNP System Board Select. Provides the state of the internal signal
//             IPPNPSYS, which is latched off the PNPCS pin at the trailing edge of
//             RESET.

// Bit 4:      VCC is 5 V. Provides the state of the internal 5-V/3.3-V detect circuitry. It is
//             High for 5 V and Low for 3.3 V.

// Bit 3:      Serial EEPROM Chip Select. If the Serial EEPROM Mode bit of the PNP
//             Serial EEPROM Enable register (PSEENI[0]) is set High, then the data
//             latched in this bit is reflected on the PNPCS pin. Reads provide the latched
//             value.

// Bit 2:      Serial EEPROM Serial Clock. Writes to this bit are reflected on the MD[2]
//             pin. Reads provide the latched value.

// Bit 1:      Serial EEPROM Data In. Writes to this bit are reflected on the MD[1] pin.
//             Reads provide the latched value.

// Bit 0:      Serial EEPROM Data Out. Writes to this bit are ignored. Reads provide the
//             state of the MD[0] pin.

typedef Has::System::PnP::Data_t EepromControl_t;
namespace EepromControl
{
    enum
    {
        DataOut = 0x01,
        DataIn = 0x02,
        Clock = 0x04,
        ChipSelect = 0x08,
        VccIs5V = 0x10,
        PnPSystemBoardSelect = 0x20,
        ExternalDecodeSelect = 0x40,
        SuspendC32KHzSelect = 0x80
    };

    inline EepromControl_t Read()
    {
        return Has::System::PnP::Data::Read(Register::EepromControl);
    }

    inline void Write(EepromControl_t eepromControl)
    {
        Has::System::PnP::Data::Write(Register::EepromControl, eepromControl);
    }

}

}