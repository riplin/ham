//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/control/irqconf.h>
#include <ham/drivers/amd/shared/pnp/device.h>

namespace Ham::Amd::Shared::PnP
{

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

typedef Has::System::PnP::InterruptType_t InterruptType_t;
namespace InterruptType
{
    enum
    {
        Type = Has::System::PnP::InterruptType::Type,
        Edge = Has::System::PnP::InterruptType::Edge,
        Level = Has::System::PnP::InterruptType::Level,
        Polarity = Has::System::PnP::InterruptType::Polarity,
        Low = Has::System::PnP::InterruptType::Low,
        High = Has::System::PnP::InterruptType::High
    };

    inline InterruptType_t ReadAudioChannel1()
    {
        Device::SelectAudio();
        return Has::System::PnP::InterruptType::Read0();
    }

    inline InterruptType_t ReadAudioChannel2()
    {
        Device::SelectAudio();
        return Has::System::PnP::InterruptType::Read1();
    }

    inline InterruptType_t ReadCdRom()
    {
        Device::SelectExternal();
        return Has::System::PnP::InterruptType::Read0();
    }

    inline InterruptType_t ReadEmulation()
    {
        Device::SelectEmulation();
        return Has::System::PnP::InterruptType::Read0();
    }

    inline InterruptType_t ReadMpu401()
    {
        Device::SelectMpu401();
        return Has::System::PnP::InterruptType::Read0();
    }

}

}