//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/control/dmaconf.h>
#include <ham/drivers/amd/shared/pnp/device.h>

namespace Ham::Amd::Shared::PnP
{

// PUD1SI, PUD2SI, PRDSI—PNP DMA Select Registers

// Address: 0A79h write, PNPRDP read; see Table 12-8 for indexes

// Default: 04h

// Bits 2–0 of these registers select the DMA request-acknowledge number as shown in
// Table 12-9.

// The DMA functions for which the DMA request-acknowledge number is selected by each
// register are:

// PUD1SI:     DMA Channel 1—local–system memory transfers or codec record

// PUD2SI:     DMA Channel 2—codec playback

// PRDSI:      External Device—passes EX_DRQ to the selected DRQ pin and selects a
//             DAK pin to control EX_DAK

// Bits 7–3 are reserved.

// Writes to PUD1SI appropriately affect the DMA Select Channel 1 field of the DMA Channel
// Control register (UDCI[2:0]). Writes to PUD2SI appropriately affect the DMA Select Channel
// 2 field of the DMA Channel Control register (UDCI[5:3]). If a value is written to either of
// these registers that is not supported by UDCI, then the corresponding UDCI field is set to
// 0 (no DMA).

// Note: It is not legal to write to this register while the IC has any DMA activity enabled.
// Do not write to this register if any of the following bits are High:
//     – Enable GUS-Compatible DMA bit of the DMA Control register (LDMACI[0])
//     – Interleaved DMA Enable bit of the LMC DMA Interleave Control register (LDICI[9])
//     – Record Enable bit of the Configuration Register 1 (CFIG1I[1])
//     – Playback Enable bit of the Configuration Register 1 (CFIG1I[0])

// Table 12-8
// Indexes for PNP DMA Select Registers
// +------------+--------+--------+-------------------------------------------+
// | Mnemonic   | PIDXR  | PLDNI  | Register Name                             |
// +------------+--------+--------+-------------------------------------------+
// | PUD1SI     | 74h    |  00h   | PNP Audio DMA Channel 1 Select            |
// | PUD2SI     | 75h    |  00h   | PNP Audio DMA Channel 2 Select            |
// | PRDSI      | 74h    |  01h   | PNP CD-ROM (External Function) DMA Select |
// +------------+--------+--------+-------------------------------------------+

// Table 12-9
// DMA Request Number Selection
// +--------+----------------+--------+-------------+
// | [2:0]  | Description    | [2:0]  | Description |
// +--------+----------------+--------+-------------+
// | 0h     | DRQ/AK0        | 4h     | No DMA      |
// | 1h     | DRQ/AK1        | 5h     | DRQ/AK5     |
// | 2h     | No DMA         | 6h     | DRQ/AK6     |
// | 3h     | DRQ/AK3        | 7h     | DRQ/AK7     |
// +--------+----------------+--------+-------------+

typedef Has::System::PnP::Data_t DmaSelect_t;
namespace DmaSelect
{
    enum
    {
        Channel0 = 0x00,
        Channel1 = 0x01,
        Channel3 = 0x02,
        NoDma = 0x04,
        Channel5 = 0x05,
        Channel6 = 0x06,
        Channel7 = 0x07
    };

    inline DmaSelect_t ReadAudioRecord()
    {
        Device::SelectAudio();
        return Has::System::PnP::DmaChannel::Read0();
    }

    inline DmaSelect_t ReadAudioPlayback()
    {
        Device::SelectAudio();
        return Has::System::PnP::DmaChannel::Read1();
    }

    inline DmaSelect_t ReadCdRom()
    {
        Device::SelectExternal();
        return Has::System::PnP::DmaChannel::Read0();
    }

    inline void WriteAudioRecord(DmaSelect_t dmaSelect)
    {
        Device::SelectAudio();
        Has::System::PnP::DmaChannel::Write0(dmaSelect);
    }

    inline void WriteAudioPlayback(DmaSelect_t dmaSelect)
    {
        Device::SelectAudio();
        Has::System::PnP::DmaChannel::Write1(dmaSelect);
    }

    inline void WriteCdRom(DmaSelect_t dmaSelect)
    {
        Device::SelectExternal();
        Has::System::PnP::DmaChannel::Write0(dmaSelect);
    }
}

}