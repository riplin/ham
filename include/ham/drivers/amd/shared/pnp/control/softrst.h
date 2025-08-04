//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/data.h>

namespace Ham::Amd::Shared::PnP
{

namespace Register
{
    enum
    {
        SoftReset = 0xF3
    };
}

// PSRSTI—PNP Software Reset

// Address: 0A79h write; index PIDXR=F3h and PLDNI=0

// This register is accessible only when the IC is in the PNP configuration state.

// Note: This register is similar to PCCCI; however, where writes to PCCCI affect all
// PNP-compliant cards in the system, writes to this register affect only the InterWave
// IC that is in the PNP configuration state.

// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+
// | Bit 7      | Bit 6          | Bit 5          | Bit 4          | Bit 3          | Bit 2          | Bit 1          | Bit 0        |
// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+
// | Reserved   | Reserved       | Reserved       | Reserved       | PnP Register   | Reset CSN      | Wait For Key   | General      |
// |            |                |                |                | Reset          |                |                | Reset        |
// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+

// Bits 7–4:   Reserved.

// Bit 3:      PNP Register Reset. Setting this bit High resets the following registers:
//                 • PNP Activate registers (PUACTI, PRACTI, PGACTI, PSACTI, PMACTI)
//                 • PNP I/O Range Check registers (PURCI, PRRCI, PGRCI, PSRCI, PMRCI)
//                 • PNP P3XR (P3X0HI, P3X0LI)
//                 • PNP PCODAR (PHCAI, PLCAI)
//                 • PNP Audio IRQ Channel Select (PUI1SI, PUI2SI)
//                 • PNP DMA Channel Select (PUD1SI, PUD2SI)
//                 • PNP Serial EEPROM Enable (PSEENI)
//                 • PNP Serial EEPROM Control (PSECI)
//                 • PNP Power Mode (PPWRI)
//                 • PNP PCDRAR (PRAHI, PRALI)
//                 • PNP PATAAR (PATAHI, PATALI)
//                 • PNP CD-ROM IRQ Type (PRISI)
//                 • PNP CD-ROM DMA Select (PRDSI)
//                 • PNP P201AR (P201HI, P201LI)
//                 • PNP P388AR (P388HI, P388LI)
//                 • PNP AdLib–Sound Blaster Emulation IRQ Select (PSBISI)
//                 • PNP P401AR (P401HI, P401LI)
//                 • PNP MPU-401 Emulation IRQ Select (PMISI)
//                 • Interrupt Control (UICI)
//                 • DMA Channel Control (UDCI)

// Bit 2:      Reset CSN. Setting this bit High sets the Card Select Number (CSN) to zero.

// Bit 1:      Wait For Key. Setting this bit High causes the IC to enter the PNP
//             wait-for-key state.

// Bit 0:      General Reset. Setting this bit High resets most of the IC. The only parts
//             of the IC not reset by this command are the PNP Set Read Data Port
//             (PSRPAI), the PNP Card Select Number register (PCSNI), the PNP state,
//             and the registers reset by bit 3 of this register.

typedef Has::System::PnP::Data_t SoftReset_t;
namespace SoftReset
{
    enum
    {
        GeneralReset = 0x01,
        WaitForKey = 0x02,
        ResetCsn = 0x04,
        PnPRegistersReset = 0x08
    };

    inline void Write(SoftReset_t softReset)
    {
        Has::System::PnP::Data::Write(Register::SoftReset, softReset);
    }
}

}