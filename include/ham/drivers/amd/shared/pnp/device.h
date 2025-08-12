//Copyright 2025-Present riplin

#pragma once

#include <has/system/pnp/control/logicdev.h>
#include <has/system/pnp/control/activate.h>

namespace Ham::Amd::Shared::PnP
{

typedef Has::System::PnP::Data_t Device_t;
namespace Device
{
    enum
    {
        Audio = Has::System::PnP::LogicalDevice::Device0,
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

    inline void SelectAudio()
    {
        using namespace Has::System::PnP;

        LogicalDevice::Write(Audio);
    }

    inline void SelectExternal()
    {
        using namespace Has::System::PnP;

        LogicalDevice::Write(External);
    }

    inline void SelectGamePort()
    {
        using namespace Has::System::PnP;

        LogicalDevice::Write(GamePort);
    }

    inline void SelectEmulation()
    {
        using namespace Has::System::PnP;

        LogicalDevice::Write(Emulation);
    }

    inline void SelectMpu401()
    {
        using namespace Has::System::PnP;

        LogicalDevice::Write(Mpu401);
    }
}

}