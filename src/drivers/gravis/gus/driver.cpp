//Copyright 2025-Present riplin

#include <new>
#include <ham/drivers/gravis/gus/gus.h>
#include <ham/drivers/gravis/gus/driver.h>

namespace Ham::Gravis::Gus
{

Driver::~Driver()
{
    Shutdown();
}

bool Driver::Detect()
{
    uint16_t baseAddress = 0;
    uint8_t playDma = 0;
    uint8_t recordDma = 0;
    uint8_t gf1Interrupt = 0;
    uint8_t midiInterrupt = 0;
    auto result = Gus::ParseEnvironmentVariable(baseAddress, playDma, recordDma, gf1Interrupt, midiInterrupt);
    if (result != InitializeError::Success)
        return false;

    if (!Gus::DetectCard(baseAddress))
        return false;

    return true;
}

Ham::Driver::Base* Driver::Create(Has::IAllocator& allocator)
{
    return ::new(allocator.Allocate(sizeof(Driver))) Driver(allocator);
}

const char* Driver::ResultToString(Ham::Driver::Result_t result)
{
    return InitializeError::ToString(result);
}

Ham::Driver::Result_t Driver::Initialize()
{
    return Gus::Initialize(m_Allocator);
}

Ham::Driver::Voice_t Driver::GetMaximumNumberOfVoices() const
{
    return 32;
}

void Driver::SetActiveVoices(Ham::Driver::Voice_t activeVoices)
{
    m_ActiveVoices = Has::bound<Ham::Driver::Voice_t>(14, activeVoices, 32);
    Gus::Configure(m_ActiveVoices);
}

void Driver::ResetMemoryManagement()
{
    Gus::ResetMemoryManagement();
}

Ham::Driver::Size_t Driver::GetAvailableMemory() const
{
    return Gus::GetAvailableMemory();
}

Ham::Driver::Address_t Driver::AllocateMemory(Ham::Driver::Size_t size)
{
    return Gus::AllocateMemory(size);
}

void Driver::UploadSound(Ham::Driver::Address_t deviceAddress, const void* data, Ham::Driver::Size_t length)
{
    Gus::UploadSound(deviceAddress, data, length);
}

void Driver::ResetVoice(Ham::Driver::Voice_t voice)
{
    Gus::ResetVoice(voice);
}

void Driver::PlayVoice(Ham::Driver::Voice_t voice, Ham::Driver::Address_t startLocation, Ham::Driver::Address_t loopStart, Ham::Driver::Address_t loopEnd, Ham::Driver::VoiceControl_t voiceControl)
{
    using namespace Ham::Driver;
    using namespace Shared::GF1;

    Voice::VoiceControl_t vc = Voice::VoiceControl::Play;
    vc |= ((voiceControl & VoiceControl::BitWidth) >> VoiceControl::Shift::BitWidth) << Voice::VoiceControl::Shift::BitWidth;
    vc |= ((voiceControl & VoiceControl::Looping) >> VoiceControl::Shift::Looping) << Voice::VoiceControl::Shift::LoopToBegin;
    vc |= ((voiceControl & VoiceControl::BidirectionalLooping) >> VoiceControl::Shift::BidirectionalLooping) << Voice::VoiceControl::Shift::BidirectionalLooping;
    vc |= ((voiceControl & VoiceControl::InterruptControl) >> VoiceControl::Shift::InterruptControl) << Voice::VoiceControl::Shift::InterruptControl;
    vc |= ((voiceControl & VoiceControl::Direction) >> VoiceControl::Shift::Direction) << Voice::VoiceControl::Shift::Direction;

    Gus::PlayVoice(voice, startLocation, loopStart, loopEnd, vc);
}

void Driver::StopVoice(Ham::Driver::Voice_t voice)
{
    Gus::StopVoice(voice);
}

void Driver::ResumeVoice(Ham::Driver::Voice_t voice)
{
    Gus::ResumeVoice(voice);
}

void Driver::SetVoiceLinearVolume(Ham::Driver::Voice_t voice, uint16_t volume)
{
    Gus::SetLinearVolume(voice, Has::min<uint16_t>(volume, 511));
}

void Driver::SetVoicePlaybackFrequency(Ham::Driver::Voice_t voice, uint16_t frequencyInHz)
{
    Gus::SetPlaybackFrequency(voice, frequencyInHz, m_ActiveVoices);
}

void Driver::SetVoicePan(Ham::Driver::Voice_t voice, Ham::Driver::PanPosition_t pan)
{
    Gus::SetPan(voice, pan >> 4);
}

}