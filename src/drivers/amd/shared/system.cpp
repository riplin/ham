//Copyright 2025-Present riplin

#include <has/testing/log.h>
#include <has/system/pnp/pnp.h>
#include <ham/drivers/amd/shared/system.h>

namespace Ham::Amd::Shared::System
{

namespace InitializeError
{

    const char* ToString(InitializeError_t error)
    {
        switch (error)
        {
        case Success:
            return "Success";
        case AlreadyInitialized:
            return "Already initialized";
        case NoCardsFound:
            return "No cards detected";
        default:
            return "Unknown error";
        }
    }
}

bool s_Initialized = false;
bool s_ShutdownPnP = false;
uint32_t s_VendorId = 0;
Has::System::PnP::CardSelectNumber_t s_Csn = 0;

//Vendor ID: 0x0496550A //The last 4 bits are a revision?   00001 00100 10110   0x41 = A 0x44 = D 0x56 = V -> ADV550A
//GRV0001 0x1E 0x56 0x00 0x01

bool DetectCard()
{
    s_VendorId = Has::System::PnP::Identifier::GRV0001;  //Ultrasound PnP

    if (!Has::System::PnP::FindDevice(s_VendorId, s_Csn))
    {
        LOG("Amd", "InterWave not found");
        return false;
    }

    LOG("Amd", "Found InterWave at csn: %i, vendor id: 0x%08lX", s_Csn, s_VendorId);
    return true;
}

InitializeError_t Initialize(Has::IAllocator& allocator)
{
    if (!s_Initialized)
    {
        Has::System::PnP::InitializeError_t result = Has::System::PnP::InitializeError::NoCardsFound;
        result = Has::System::PnP::Initialize(allocator);
        s_ShutdownPnP = result == Has::System::PnP::InitializeError::Success;
        if ((result == Has::System::PnP::InitializeError::Success) ||
            (result == Has::System::PnP::InitializeError::AlreadyInitialized))
        {
            if (!DetectCard())
            {
                result = Has::System::PnP::InitializeError::NoCardsFound;
            }
            else
            {
                //TODO: Init
            }
        }

        if (result == Has::System::PnP::InitializeError::NoCardsFound)
            return InitializeError::NoCardsFound;

        s_Initialized = true;
        return InitializeError::Success;
    }
    return InitializeError::AlreadyInitialized;
}

void Shutdown()
{
    if (s_Initialized)
    {


        if (s_ShutdownPnP)
            Has::System::PnP::Shutdown();

        s_ShutdownPnP = false;
        s_Initialized = false;
    }
}

}