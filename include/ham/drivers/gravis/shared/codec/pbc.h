//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/gravis/shared/codec/data.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::PlaybackBaseCount8_t PlaybackBaseCount8_t;
typedef Crystal::CS4231::PlaybackBaseCount16_t PlaybackBaseCount16_t;
namespace PlaybackBaseCount
{
   
    inline PlaybackBaseCount8_t ReadUpper(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::PlaybackUpperBaseCount);
    }

    inline PlaybackBaseCount8_t ReadLower(Register_t baseAddress)
    {
        return Data::Read(baseAddress, Crystal::CS4231::Register::PlaybackLowerBaseCount);
    }

    inline PlaybackBaseCount16_t Read(Register_t baseAddress)
    {
        return (PlaybackBaseCount16_t(Data::Read(baseAddress, Crystal::CS4231::Register::PlaybackUpperBaseCount)) << 8) |
                Data::Read(baseAddress, Crystal::CS4231::Register::PlaybackLowerBaseCount);
    }

    inline void WriteUpper(Register_t baseAddress, PlaybackBaseCount8_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::PlaybackUpperBaseCount, value);
    }

    inline void WriteLower(Register_t baseAddress, PlaybackBaseCount8_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::PlaybackLowerBaseCount, value);
    }

    inline void Write(Register_t baseAddress, PlaybackBaseCount16_t value)
    {
        Data::Write(baseAddress, Crystal::CS4231::Register::PlaybackUpperBaseCount, value >> 16);
        Data::Write(baseAddress, Crystal::CS4231::Register::PlaybackLowerBaseCount, PlaybackBaseCount8_t(value));
    }

}

}