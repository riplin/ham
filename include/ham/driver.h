//Copyright 2025-Present riplin

#pragma once

#include <has/ialloc.h>

namespace Ham::Driver
{

typedef uint16_t Result_t;
namespace Result
{
    enum Result
    {
        Success = 0x0000
    };
}

typedef uint8_t Voice_t;
typedef uint32_t Size_t;
typedef uint32_t Address_t;

typedef uint16_t Volume_t;
namespace Volume
{
    enum
    {
        Min = 0,
        Max = 511
    };
}

typedef uint8_t PanPosition_t;
namespace PanPosition
{
    enum
    {
        Left = 0x00,
        Middle = 0x7F,
        Right = 0xFF
    };
}

typedef uint8_t VoiceControl_t;
namespace VoiceControl
{
    enum
    {
        BitWidth = 0x04,
        Bit8 = 0x00,
        Bit16 = 0x04,
        Looping = 0x08,
        BidirectionalLooping = 0x10,
        InterruptControl = 0x20,
        InterruptDisable = 0x00,
        InterruptEnable = 0x20,
        Direction = 0x40,
        Forward = 0x00,
        Backward = 0x40
    };

    namespace Shift
    {
        enum
        {
            BitWidth = 0x02,
            Looping = 0x03,
            BidirectionalLooping = 0x04,
            InterruptControl = 0x05,
            Direction = 0x06,
        };
    }
}

class Base
{
public:

    static void Destroy(Base* base);

    virtual const char* ResultToString(Result_t result) = 0;

    virtual Result_t Initialize() = 0;

    virtual const char* GetName() const = 0;

    virtual Voice_t GetMaximumNumberOfVoices() const = 0;
    virtual void SetActiveVoices(Voice_t activeVoices) = 0;

    virtual void ResetMemoryManagement() = 0;
    virtual Size_t GetAvailableMemory() const = 0;
    virtual Address_t AllocateMemory(Size_t size) = 0;
    virtual void UploadSound(Address_t deviceAddress, const void* data, Size_t length) = 0;

    virtual void ResetVoice(Voice_t voice) = 0;
    virtual void PlayVoice(Voice_t voice, Address_t startLocation, Address_t loopStart, Address_t loopEnd, VoiceControl_t voiceControl) = 0;
    virtual void StopVoice(Voice_t voice) = 0;
    virtual void ResumeVoice(Voice_t voice) = 0;
    virtual void SetVoiceLinearVolume(Voice_t voice, uint16_t volume) = 0;
    virtual void SetVoicePlaybackFrequency(Voice_t voice, uint16_t frequencyInHz) = 0;
    virtual void SetVoicePan(Voice_t voice, PanPosition_t pan) = 0;

    HAS_NOCOPY(Base);
protected:
    inline Base(Has::IAllocator& allocator)
        : m_Allocator(allocator)
    {
    }

    virtual inline ~Base()
    {
    }

    Has::IAllocator& m_Allocator;
};

}