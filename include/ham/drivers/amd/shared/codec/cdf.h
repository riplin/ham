//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/amd/shared/codec/regtype.h>

namespace Ham::Amd::Shared::Codec
{

// +---------------+----------------+----------------+----------------+----------------+----------------+----------------+----------+
// | Bit 7         | Bit 6          | Bit 5          | Bit 4          | Bit 3          | Bit 2          | Bit 1          | Bit 0    |
// +---------------+----------------+----------------+----------------+----------------+----------------+----------------+----------+
// | Record Data Format Selection                    | Record         | Record Clock Divider Select                      | Record   |
// |                                                 | Stereo/Mono    |                                                  | Crystal  |
// |                                                 |                |                                                  | Select   |
// +---------------+----------------+----------------+----------------+----------------+----------------+----------------+----------+


typedef Crystal::CS4231::CaptureDataFormat_t CaptureDataFormat_t;
namespace CaptureDataFormat
{
    enum
    {
        ClockSource = 0x01,
        Clock24p576 = 0x00,
        Clock16p9344 = 0x01,
        ClockFrequencyDivideSelect = 0x0E,
        Channels = Crystal::CS4231::CaptureDataFormat::Channels,
        Mono = Crystal::CS4231::CaptureDataFormat::Mono,
        Stereo = Crystal::CS4231::CaptureDataFormat::Stereo,
        SignalType = Crystal::CS4231::CaptureDataFormat::SignalType,
        Linear = Crystal::CS4231::CaptureDataFormat::Linear,
        Companded = Crystal::CS4231::CaptureDataFormat::Companded,
        Format = Crystal::CS4231::CaptureDataFormat::Format,
        Unsigned8Bit = Crystal::CS4231::CaptureDataFormat::Unsigned8Bit,
        Signed16bit = Crystal::CS4231::CaptureDataFormat::Signed16bit,
        uLaw = Crystal::CS4231::CaptureDataFormat::uLaw,
        aLaw = Crystal::CS4231::CaptureDataFormat::aLaw
    };
}

namespace IO::CaptureDataFormat
{
    inline CaptureDataFormat_t Read(Register_t baseAddress)
    {
        return Crystal::CS4231::IO::CaptureDataFormat::Read(baseAddress);
    }
    
    inline void Write(Register_t baseAddress, CaptureDataFormat_t value)
    {
        Crystal::CS4231::IO::CaptureDataFormat::Write(baseAddress, value);
    }
}

}