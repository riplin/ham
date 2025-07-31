//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/shared/data.h>

namespace Ham::Crystal::Shared
{

namespace Register
{
    enum
    {
        VersionAndChipId = 0x19,
    };
}

// Version / ID (I25)

// +------+------+------+-----+------+------+------+------+
// |  D7  |  D6  |  D5  |  D4 |  D3  |  D2  |  D1  |  DO  |
// +------+------+------+-----+------+------+------+------+
// |  V2     V1     V0  | res | res  | CID2   CID1   CID0 |
// +------+------+------+-----+------+------+------+------+

// V2-V0       Version number. As enhancements are
//             made to the CS4231, the version
//             number is changed so software can
//             distinguish between the different
//             versions.

//                 100 - Revision C, D, & E. This Data
//                     Sheet

// CID2-CIDO   Chip Identification. Distinguishes
//             between this chip and future chips
//             that support this register set.
//                 000 - CS4231

// This register's initial state after reset is: 000xx000

typedef uint8_t VersionAndChipId_t;
namespace VersionAndChipId
{

    enum
    {
        ChipId = 0x07,
        RevisionCDE = 0x80,
        Version = 0xE0,
        CS4231 = 0x00
    };

    namespace Shift
    {
        enum
        {
            ChipId = 0x00,
            Version = 0x05
        };
    };

}

}