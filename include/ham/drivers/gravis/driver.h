//Copyright 2025-Present riplin

#pragma once

#include <ham/driver.h>

namespace Ham::Gravis
{

class Driver : public Ham::Driver::Base
{
public:
    static bool Detect();
    static Ham::Driver::Base* Create(Has::IAllocator& allocator);

    virtual const char* ResultToString(Ham::Driver::Result_t result) override;

    virtual Ham::Driver::Result_t Initialize() override;

    virtual const char* GetName() const override;

    virtual Ham::Driver::Voice_t GetMaximumNumberOfVoices() const override;
    virtual void SetActiveVoices(Ham::Driver::Voice_t activeVoices) override;

    virtual void ResetMemoryManagement() override;
    virtual Ham::Driver::Size_t GetAvailableMemory() const override;
    virtual Ham::Driver::Address_t AllocateMemory(Ham::Driver::Size_t size) override;
    virtual void UploadSound(Ham::Driver::Address_t deviceAddress, const void* data, Ham::Driver::Size_t length) override;

    virtual void ResetVoice(Ham::Driver::Voice_t voice) override;
    virtual void PlayVoice(Ham::Driver::Voice_t voice, Ham::Driver::Address_t startLocation, Ham::Driver::Address_t loopStart, Ham::Driver::Address_t loopEnd, Ham::Driver::VoiceControl_t voiceControl) override;
    virtual void StopVoice(Ham::Driver::Voice_t voice) override;
    virtual void ResumeVoice(Ham::Driver::Voice_t voice) override;
    virtual void SetVoiceLinearVolume(Ham::Driver::Voice_t voice, uint16_t volume) override;
    virtual void SetVoicePlaybackFrequency(Ham::Driver::Voice_t voice, uint16_t frequencyInHz) override;
    virtual void SetVoicePan(Ham::Driver::Voice_t voice, Ham::Driver::PanPosition_t pan) override;

    HAS_NOCOPY(Driver);
protected:
    inline Driver(Has::IAllocator& allocator)
        : Base(allocator)
        , m_ActiveVoices(14)
        {
        }

    virtual ~Driver();

    Ham::Driver::Voice_t m_ActiveVoices;
};

class DriverMixer : public Driver
{
public:
    inline DriverMixer(Has::IAllocator& allocator)
        : Driver(allocator)
    {
    }

    virtual const char* GetName() const override;

};

class DriverMixerReversed : public Driver
{
public:
    inline DriverMixerReversed(Has::IAllocator& allocator)
        : Driver(allocator)
    {
    }

    virtual const char* GetName() const override;

};

class DriverCodec : public Driver
{
public:
    inline DriverCodec(Has::IAllocator& allocator)
        : Driver(allocator)
    {
    }

    virtual const char* GetName() const override;

};

}