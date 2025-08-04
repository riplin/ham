//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/control/logicdev.h>
#include <ham/drivers/amd/shared/pnp/control/activate.h>

namespace Ham::Amd::Shared::PnP
{

typedef Has::System::PnP::Data_t Device_t;
namespace Device
{
    enum
    {
        Audio = Has::System::PnP::LogicalDevice::Card,
        External = Has::System::PnP::LogicalDevice::Device1,
        GamePort = Has::System::PnP::LogicalDevice::Device2,
        Emulation = Has::System::PnP::LogicalDevice::Device3,
        Mpu401 = Has::System::PnP::LogicalDevice::Device4
    };

    // Assumes card is in the configuration state.
    // Send the key and wake the card to put it in the configuration state.
    inline void Enable(Device_t device)
    {
        using namespace Has::System::PnP;

        SYS_ClearInterrupts();
        LogicalDevice::Write(device);
        Activate::Write(Activate::Enable);
        SYS_RestoreInterrupts();
    }

    // Assumes card is in the configuration state.
    // Send the key and wake the card to put it in the configuration state.
    inline void Disable(Device_t device)
    {
        using namespace Has::System::PnP;

        SYS_ClearInterrupts();
        LogicalDevice::Write(device);
        Activate::Write(Activate::Disable);
        SYS_RestoreInterrupts();
    }

}

}