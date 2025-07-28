//Copyright 2025-Present riplin

#pragma once

#include <has/ialloc.h>
#include <ham/drivers/creative/shared/regtype.h>

namespace Ham::Creative::Shared::Functions::System
{

    typedef uint16_t InitializeError_t;
    namespace InitializeError
    {
        enum
        {
            Success = 0x00,
            AlreadyInitialized = 0x01,
            BlasterEnvNotFound = 0x02,
            BlasterEnvMalformed = 0x03,
            BlasterEnvInvalidValues = 0x04,
            CardNotFound = 0x05
        };

        extern const char* ToString(InitializeError_t error);
    }

    typedef uint8_t CardType_t;
    namespace CardType
    {
        enum
        {
            Unknown = 0x00,
            SoundBlaster = 0x01,
            SoundBlaster1p5 = 0x02,
            SoundBlaster2 = 0x03,
            SoundBlasterPro = 0x04,
            SoundBlasterProWithFM = 0x05,
            SoundBlaster16 = 0x06
        };
    }

    extern bool DetectCard(Register_t baseAddress);

    extern InitializeError_t ParseEnvironmentVariable(Register_t& baseAddress, uint8_t& lowDma, uint8_t& highDma, uint8_t& interrupt, Register_t& mixerBase, Register_t& midiBase, CardType_t& type, Register_t& emuBase);

    extern InitializeError_t Initialize(Has::IAllocator& allocator);
    extern void Shutdown();

    extern uint32_t GetMemorySize();

}