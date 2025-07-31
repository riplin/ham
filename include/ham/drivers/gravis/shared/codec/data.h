//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/gravis/shared/codec/index.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::Data_t Data_t;
namespace Data
{

    inline Data_t Read(Register_t baseAddress)
    {
        return Crystal::CS4231::Data::Read(baseAddress + Register::Base);
    }
    
    inline Data_t Read(Register_t baseAddress, Index_t index)
    {
        Index::Write(baseAddress, index);
        return Read(baseAddress);
    }

    inline void Write(Register_t baseAddress, Data_t value)
    {
        Crystal::CS4231::Data::Write(baseAddress + Register::Base, value);
    }
    
    inline void Write(Register_t baseAddress, Index_t index, Data_t value)
    {
        Index::Write(baseAddress, index);
        Write(baseAddress, value);
    }

}

}