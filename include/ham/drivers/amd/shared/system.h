//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Ham::Amd::Shared::System
{

typedef uint16_t InitializeError_t;
namespace InitializeError
{
    enum
    {
        Success = 0x00,
        AlreadyInitialized = 0x01,
        NoCardsFound = 0x02,
    };
}

extern InitializeError_t Initialize();
extern void Shutdown();

}