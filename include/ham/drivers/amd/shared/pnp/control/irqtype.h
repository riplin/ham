//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/data.h>

namespace Ham::Amd::Shared::PnP
{

namespace Register
{
    enum
    {
        AudioChannel1InterruptType = 0x71,
        AudioChannel2InterruptType = 0x73,
        CdRomInterruptType = 0x71,
        EmulationInterruptType = 0x71,
        Mpu401InterruptType = 0x71
    };
}

// PUI1TI, PUI2TI, PRITI, PSBITI, PMITI—PNP IRQ Type Registers

// Address: PNPRDP read; see Table 12-4 for indexes

// Default: 02h

// These registers provide data back to standard PNP software concerning the type of
// interrupts supported by the InterWave IC. Each register always reads as 02h to indicate
// edge-triggered, active-high interrupts.

// Table 12-7
// Indexes for PNP IRQ Type Registers
// +------------+--------+--------+--------------------------------------------+
// | Mnemonic   | PIDXR  | PLDNI  | Register Name                              |
// +------------+--------+--------+--------------------------------------------+
// | PUI1TI     | 71h    | 00h    | PNP Audio IRQ Channel 1 Type               |
// | PUI2TI     | 73h    | 00h    | PNP Audio IRQ Channel 2 Type               |
// | PRITI      | 71h    | 01h    | PNP CD-ROM (External Function) IRQ Type    |
// | PSBITI     | 71h    | 03h    | PNP AdLib–Sound Blaster Emulation IRQ Type |
// | PMITI      | 71h    | 04h    | PNP MPU-401 Emulation IRQ Type             |
// +------------+--------+--------+--------------------------------------------+

typedef Has::System::PnP::Data_t InterruptType_t;
namespace InterruptType
{
    enum
    {
        EdgeTriggered = 0x02
    };

    inline InterruptType_t ReadAudioChannel1()
    {
        return Has::System::PnP::Data::Read(Register::AudioChannel1InterruptType);
    }

    inline InterruptType_t ReadAudioChannel2()
    {
        return Has::System::PnP::Data::Read(Register::AudioChannel2InterruptType);
    }

    inline InterruptType_t ReadCdRom()
    {
        return Has::System::PnP::Data::Read(Register::CdRomInterruptType);
    }

    inline InterruptType_t ReadEmulation()
    {
        return Has::System::PnP::Data::Read(Register::EmulationInterruptType);
    }

    inline InterruptType_t ReadMpu401()
    {
        return Has::System::PnP::Data::Read(Register::Mpu401InterruptType);
    }

}

}