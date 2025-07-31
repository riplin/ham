//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/gravis/shared/regtype.h>
#include <ham/drivers/gravis/shared/codec/base.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::Index_t Index_t;
namespace Index
{

    enum
    {
        Address = Crystal::CS4231::Index::Address,
        TransferRequest = Crystal::CS4231::Index::TransferRequest,
        TransferRequestEnable = Crystal::CS4231::Index::TransferRequestEnable,
        TransferRequestDisable = Crystal::CS4231::Index::TransferRequestDisable,
        ModeChange = Crystal::CS4231::Index::ModeChange,
        ModeChangeDisable = Crystal::CS4231::Index::ModeChangeDisable,
        ModechangeEnable = Crystal::CS4231::Index::ModechangeEnable,
        Initialize = Crystal::CS4231::Index::Initialize
    };
    
    namespace Shift
    {
        using namespace Crystal::CS4231::Index::Shift;
    }

    inline Index_t Read(Register_t baseAddress)
    {
        return Crystal::CS4231::Index::Read(baseAddress + Register::Base);
    }

    inline void Write(Register_t baseAddress, Index_t value)
    {
        Crystal::CS4231::Index::Write(baseAddress + Register::Base, value);
    }

}

}