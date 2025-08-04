//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/data.h>

namespace Ham::Amd::Shared::PnP
{

namespace Register
{
    enum
    {
        RangeCheck = 0x31
    };
}

// PURCI, PRRCI, PGRCI, PSRCI, PMRCI—PNP I/O Range Check Registers

// Address: 0A79h write, PNPRDP read; indexes PIDXR=31h and PLDNI= one of the
// following:
//     0:PURCI (audio)
//     1:PRRCI (external)
//     2:PGRCI (game port)
//     3:PSRCI (AdLib–Sound Blaster)
//     4:PMRCI (MPU-401)

// Default: 00h

// Note: These registers are not available when the IC is in external decoding mode.
// These registers are disabled when bit 0 of any of the corresponding activate registers
// (PUACTI, PRACTI, PGACTI, PSACTI, PMACTI) is set High.

// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+
// | Bit 7      | Bit 6          | Bit 5          | Bit 4          | Bit 3          | Bit 2          | Bit 1          | Bit 0        |
// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+
// | reserved   | reserved       | reserved       | reserved       | reserved       | reserved       | Range Check    | High 0x55 /  |
// |            |                |                |                |                |                | Enable         | Low 0xAA     |
// |            |                |                |                |                |                |                |              |
// +------------+----------------+----------------+----------------+----------------+----------------+----------------+--------------+

// Bits 7–2:   Reserved.

// Bit 1:      Range Check Enable. If set High, causes reads of all of the specified logical
//             device address spaces to drive either 55h or AAh based on the state of the
//             High 55h/Low AAh field. This bit functions only when the logical device is
//             not activated

// Bit 0:      High 55h/Low AAh. When the Range Check Enable bit is High (PURCI[1]),
//             this bit selects the data value that is driven back onto the ISA data bus during
//             a read. Setting this bit High specifies that 55h be driven; Low specifies AAh.

typedef Has::System::PnP::Data_t RangeCheck_t;
namespace RangeCheck
{
    enum
    {
        Value = 0x01,
        Drive0xAA = 0x00,
        Drive0x55 = 0x01,
        Control = 0x02,
        Disable = 0x00,
        Enable = 0x02
    };

    inline RangeCheck_t Read()
    {
        return Has::System::PnP::Data::Read(Register::RangeCheck);
    }

    inline void Write(RangeCheck_t rangeCheck)
    {
        Has::System::PnP::Data::Write(Register::RangeCheck, rangeCheck);
    }

    // Assumes card is in the configuration state.
    // Send the key and wake the card to put it in the configuration state.
    // The device associated with the port rage must be disabled.
    inline bool RangeCheck(Has::System::PnP::Register_t baseAddress, uint8_t portCount)
    {
        using namespace Has::System::PnP;
        Address::Write(Register::RangeCheck);

        for (uint8_t port = 0; port < portCount; ++port)
        {
            Write(Enable | Drive0xAA);
            if (SYS_ReadPortByte(baseAddress + port) != 0xAA)
                return false; // Port conflict. Someone else is answering.

            Write(Enable | Drive0x55);
            if (SYS_ReadPortByte(baseAddress + port) != 0x55)
                return false; // Port conflict. Someone else is answering.
        }

        return true;
    }
}

}