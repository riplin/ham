//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/data.h>

namespace Ham::Amd::Shared::PnP
{

namespace Register
{
    enum
    {
        Activate = 0x30
    };
}

// PUACTI, PRACTI, PGACTI, PSACTI, PMACTI—PNP Activate Registers

// Address:    0A79h write, PNPRDP read; indexes PIDXR = 30h and PLDNI = one of the
//             following:
//                 0:PUACTI (audio)
//                 1:PRACTI (external)
//                 2:PGACTI (game port)
//                 3:PSACTI (AdLib–Sound Blaster)
//                 4:PMACTI (MPU-401)

// Default: 00h

// Setting bit 0 High activates the specified logical device; all other bits are reserved. If set
// Low, none of the specified logical device’s address spaces are decoded and the interrupt
// and DMA channels are not enabled.

typedef Has::System::PnP::Data_t Activate_t;
namespace Activate
{
    enum
    {
        Control = 0x01,
        Disable = 0x00,
        Enable = 0x01
    };

    inline Activate_t Read()
    {
        return Has::System::PnP::Data::Read(Register::Activate);
    }

    inline void Write(Activate_t activate)
    {
        Has::System::PnP::Data::Write(Register::Activate, activate);
    }

}

}