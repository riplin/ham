//Copyright 2025-Present riplin

#pragma once

#include <functional>
#include <has/ialloc.h>
#include <ham/drivers/gravis/shared/midi/data.h>
#include <ham/drivers/gravis/shared/midi/status.h>

namespace Ham::Gravis::Shared::Function::System
{
    typedef uint16_t InitializeError_t;
    namespace InitializeError
    {
        enum
        {
            Success = 0x00,
            AlreadyInitialized = 0x01,
            UltrasoundEnvNotFound = 0x02,
            UltrasoundEnvMalformed = 0x03,
            UltrasoundEnvInvalidValues = 0x04,
            CardNotFound = 0x05
        };

        extern const char* ToString(InitializeError_t error);
    }

    extern InitializeError_t Initialize(Has::IAllocator& allocator);
    extern void Shutdown();


    typedef std::function<void(void)> TimerCallback_t;

    extern bool SetTimer1Handler(const TimerCallback_t& callback, uint8_t ticksPerSecond);
    extern bool SetTimer2Handler(const TimerCallback_t& callback, uint8_t ticksPerSecond);


    typedef std::function<void(Midi::Status_t status)> MidiTransmitCallback_t;
    
    extern void SetMidiTransmitCallback(const MidiTransmitCallback_t& callback);


    typedef std::function<void(Midi::Status_t status, Midi::Data_t data)> MidiReceiveCallback_t;

    extern void SetMidiReceiveCallback(const MidiReceiveCallback_t& callback);
}