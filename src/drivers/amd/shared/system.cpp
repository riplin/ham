//Copyright 2025-Present riplin

#include <has/system/pnp/pnp.h>
#include <has/testing/log.h>
#include <ham/drivers/amd/shared/system.h>

namespace Ham::Amd::Shared::System
{

bool s_Initialized = false;
bool s_ShutdownPnP = false;
uint32_t s_VendorId = 0;
Has::System::PnP::CardSelectNumber_t s_Csn = 0;

//Vendor ID: 0x0496550A //The last 4 bits are a revision?

bool DetectCard()
{
    using namespace Has::System;

    s_VendorId = 0x0496550A;

    if (!PnP::FindDevice(s_VendorId, s_Csn))
    {
        LOG("Amd", "InterWave not found");
        return false;
    }

    LOG("Amd", "Found InterWave at csn: %i, vendor id: 0x%08lX", s_Csn, s_VendorId);
    return true;
}

InitializeError_t Initialize()
{
    using namespace Has::System;
    if (!s_Initialized)
    {
        PnP::InitializeError_t result = PnP::InitializeError::NoCardsFound;
        if (!DetectCard()) //See if PnP devices have already been set up.
        {
            LOG("Amd", "Card not found or not initialized. Attempting to initialize PnP cards...");
            result = PnP::Initialize(true); // Full PnP device initialization.
            s_ShutdownPnP = result == PnP::InitializeError::Success;
            if ((result == PnP::InitializeError::Success) || (result == PnP::InitializeError::AlreadyInitialized))
            {
                if (!DetectCard())
                {
                    result = PnP::InitializeError::NoCardsFound;
                }



            }
        }
        else
        {
            result = PnP::InitializeError::Success;
        }

        if (result == PnP::InitializeError::NoCardsFound)
            return InitializeError::NoCardsFound;




        s_Initialized = true;
        return InitializeError::Success;
    }
    return InitializeError::AlreadyInitialized;
}

void Shutdown()
{
    using namespace Has::System;

    if (s_Initialized)
    {


        if (s_ShutdownPnP)
            PnP::Shutdown();

        s_ShutdownPnP = false;
        s_Initialized = false;
    }
}

}