//Copyright 2025-Present riplin

#pragma once

#include <has/ialloc.h>

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

    extern const char* ToString(InitializeError_t error);
}

extern InitializeError_t Initialize(Has::IAllocator& allocator);
extern void Shutdown();

}