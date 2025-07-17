//Copyright 2025-Present riplin

#pragma once

#include <functional>
#include <has/ialloc.h>

namespace Ham::Gravis::Shared::Function::System
{
    typedef uint16_t InitializeError_t;
    namespace InitializeError
    {
        enum
        {
            Success = 0x00,
            UltrasoundEnvNotFound = 0x01,
            UltrasoundEnvMalformed = 0x02,
            UltrasoundEnvInvalidValues = 0x03,
            CardNotFound = 0x04
        };

        extern const char* ToString(InitializeError_t error);
    }

    extern InitializeError_t Initialize(Has::IAllocator& allocator);
    extern void Shutdown();

    typedef std::function<void(void)> TimerCallback_t;

    extern bool SetTimer1Handler(const TimerCallback_t& callback, uint8_t ticksPerSecond);
    extern bool SetTimer2Handler(const TimerCallback_t& callback, uint8_t ticksPerSecond);
}