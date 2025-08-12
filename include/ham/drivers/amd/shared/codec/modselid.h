//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/amd/shared/codec/regtype.h>

namespace Ham::Amd::Shared::Codec
{

// CMODEI—Mode Select, ID

// Address: PCODAR+1 read, write; index CIDXR[4:0]=Ch

// Default: 100X 1010

// Modes: 1, 2, and 3

// This register specifies the operating mode of the codec and reports the revision number of
// the InterWave IC.

// +---------------+---------------------------------+----------------+-------------------------------------------------------------+
// | Bit 7         | Bit 6            Bit 5          | Bit 4          | Bit 3            Bit 2            Bit 1            Bit 0    |
// +---------------+---------------------------------+----------------+-------------------------------------------------------------+
// | Revision ID   | Mode Select                     | Reserved       | Revision ID Number Bits 3-0                                 |
// | Number Bit 4  |                                 |                |                                                             |
// |               |                                 |                |                                                             |
// +---------------+---------------------------------+----------------+-------------------------------------------------------------+

// Bits 7, 3–0:    Revision ID Number. These five bits specify the revision number of the IC,
//                 which is 1,1010 for the first AMD part. These bits are read-only; they cannot
//                 be changed.

// Bits 6–5:       Mode Select. To enter mode 3, write 6Ch to this port; bit 5 is forced Low
//                 for writes of any other value.
//                     00:mode 1
//                     10:mode 2
//                     01:reserved
//                     11:mode 3

// Bit 4:          Reserved.

typedef Crystal::CS4231::ModeAndId_t ModeSelectId_t;
namespace ModeSelectId
{
    enum
    {
        ChipVersion = Crystal::CS4231::ModeAndId::ChipVersion | 0x80,
        CrystalVersionB = Crystal::CS4231::ModeAndId::VersionB,
        CrystalVersionCup = Crystal::CS4231::ModeAndId::VersionCup,
        ModeControl = Crystal::CS4231::ModeAndId::Mode2Control | 0x20,
        Mode1 = Crystal::CS4231::ModeAndId::Mode2Disable,
        Mode2 = Crystal::CS4231::ModeAndId::Mode2Enable,
        Mode3 = 0x60
    };
}

namespace IO::ModeSelectId
{
    inline ModeSelectId_t Read(Register_t baseAddress)
    {
        return Crystal::CS4231::IO::ModeAndId::Read(baseAddress);
    }
    
    inline void Write(Register_t baseAddress, ModeSelectId_t value)
    {
        Crystal::CS4231::IO::ModeAndId::Write(baseAddress, value);
    }
}

}