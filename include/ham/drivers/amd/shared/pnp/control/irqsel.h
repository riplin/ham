//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/control/irqconf.h>
#include <ham/drivers/amd/shared/pnp/device.h>

namespace Ham::Amd::Shared::PnP
{

// PUI1SI, PUI2SI, PRISI, PSBISI, PMISI—PNP IRQ Select Registers

// Address: 0A79h write, PNPRDP read; see Table 12-4 for indexes

// Default: 00h

// Bits 3–0 select the IRQ number for the specified logical device interrupts as shown in
// Table 12-5. The mapping of IRQ number to interrupt event for each register is shown in
// Table 12-6.

// IRQ10 and IRQ4 are not available unless selected. For details, see the description of bit 7
// in “IEIRQI—Emulation IRQ” on page 12-20.

// Bits 7–4 of each of these registers are reserved.

// Writes to PUI1SI appropriately affect the Channel 1 IRQ Selection field of the Interrupt
// Control register (UICI[2:0]). If a value is written to PUI1SI that is not supported by UICI[2:0],
// then UICI[2:0] is set to 7 (IRQ15). Writes to PUI2SI appropriately affect the Channel 2 IRQ
// Selection field of the Interrupt Control register (UICI[5:3]). If a value is written to PUI2SI
// that is not supported by UICI[5:3], then UICI[5:3] is set to 0 (no IRQ).

// Table 12-4
// Indexes for PNP IRQ Select Registers
// +------------+--------+--------+----------------------------------------------+
// | Mnemonic   | PIDXR  | PLDNI  | Register Name                                |
// +------------+--------+--------+----------------------------------------------+
// | PUI1SI     | 70h    | 00h    | PNP Audio IRQ Channel 1 Select               |
// | PUI2SI     | 72h    | 00h    | PNP Audio IRQ Channel 2 Select               |
// | PRISI      | 70h    | 01h    | PNP CD-ROM (External Function) IRQ Select    |
// | PSBISI     | 70h    | 03h    | PNP AdLib–Sound Blaster Emulation IRQ Select |
// | PMISI      | 70h    | 04h    | PNP MPU-401 Emulation IRQ Select             |
// +------------+--------+--------+----------------------------------------------+

// Table 12-5
// IRQ Number Selection
// +--------+----------------+--------+----------------+--------+----------------+--------+-------------+
// | [3:0]  | Description    | [3:0]  | Description    | [3:0]  | Description    | [3:0]  | Description |
// +--------+----------------+--------+----------------+--------+----------------+--------+-------------+
// | 0h     | No IRQ         | 4h     | IRQ4           | 8h     | No IRQ         | 0Ch    | IRQ12       |
// | 1h     | No IRQ         | 5h     | IRQ5           | 9h     | IRQ2/9         | 0Dh    | No IRQ      |
// | 2h     | IRQ2/9         | 6h     | No IRQ         | 0Ah    | IRQ10          | 0Eh    | No IRQ      |
// | 3h     | IRQ3           | 7h     | IRQ7           | 0Bh    | IRQ11          | 0Fh    | IRQ15       |
// +--------+----------------+--------+----------------+--------+----------------+--------+-------------+

// Table 12-6
// IRQ Number to Interrupt Event Mapping for IRQ Select Registers
// +------------+--------------------------------------------------------------------------------+
// | Mnemonic   | Selects the IRQ number for the following interrupts                            |
// +------------+--------------------------------------------------------------------------------+
// | PUI1SI     | Audio Channel 1 IRQs; normally passes along synthesizer, codec, and various    |
// |            | miscellaneous IRQs                                                             |
// | PUI2SI     | Audio Channel 2 IRQs; normally passes along the MIDI IRQs; not used in many    |
// |            | configurations                                                                 |
// | PRISI      | External function (e.g., CD-ROM) IRQ. This IRQ is sourced from the EX_IRQ pin. |
// | PSBISI     | AdLib–Sound Blaster emulation IRQ. This IRQ is fully controlled by the IRQ     |
// |            | Sound Blaster bit of the Emulation IRQ register (IEIRQI[0]).                   |
// | PMISI      | MPU-401 emulation IRQ. This IRQ is fully controlled by the IRQ MPU-401 bit of  |
// |            | the Emulation IRQ register (IEIRQI[1]).                                        |
// +------------+--------------------------------------------------------------------------------+

typedef Has::System::PnP::Interrupt_t InterruptSelect_t;
namespace InterruptSelect
{
    enum
    {
        NoInterrupt = 0x00,
        Interrupt3 = 0x03,
        Interrupt4 = 0x04,
        Interrupt5 = 0x05,
        Interrupt7 = 0x07,
        Interrupt9 = 0x09,
        Interrupt10 = 0x0A,
        Interrupt11 = 0x0B,
        Interrupt12 = 0x0C,
        Interrupt15 = 0x0F
    };

    inline InterruptSelect_t ReadAudioChannel1()
    {
        Device::SelectAudio();
        return Has::System::PnP::Interrupt::Read0();
    }

    inline InterruptSelect_t ReadAudioChannel2()
    {
        Device::SelectAudio();
        return Has::System::PnP::Interrupt::Read1();
    }

    inline InterruptSelect_t ReadCdRom()
    {
        Device::SelectExternal();
        return Has::System::PnP::Interrupt::Read0();
    }

    inline InterruptSelect_t ReadEmulation()
    {
        Device::SelectEmulation();
        return Has::System::PnP::Interrupt::Read0();
    }

    inline InterruptSelect_t ReadMpu401()
    {
        Device::SelectMpu401();
        return Has::System::PnP::Interrupt::Read0();
    }

    inline void WriteAudioChannel1(InterruptSelect_t interruptSelect)
    {
        Device::SelectAudio();
        Has::System::PnP::Interrupt::Write0(interruptSelect);
    }

    inline void WriteAudioChannel2(InterruptSelect_t interruptSelect)
    {
        Device::SelectAudio();
        Has::System::PnP::Interrupt::Write1(interruptSelect);
    }

    inline void WriteCdRom(InterruptSelect_t interruptSelect)
    {
        Device::SelectExternal();
        Has::System::PnP::Interrupt::Write0(interruptSelect);
    }

    inline void WriteEmulation(InterruptSelect_t interruptSelect)
    {
        Device::SelectEmulation();
        Has::System::PnP::Interrupt::Write0(interruptSelect);
    }

    inline void WriteMpu401(InterruptSelect_t interruptSelect)
    {
        Device::SelectMpu401();
        Has::System::PnP::Interrupt::Write0(interruptSelect);
    }

}

}