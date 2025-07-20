//Copyright 2025-Present riplin

#pragma once

#include <functional>
#include <has/ialloc.h>
#include <ham/drivers/gravis/shared/gf1/page.h>
#include <ham/drivers/gravis/shared/midi/data.h>
#include <ham/drivers/gravis/shared/midi/status.h>
#include <ham/drivers/gravis/shared/gf1/voice/panpos.h>
#include <ham/drivers/gravis/shared/gf1/voice/curvol.h>
#include <ham/drivers/gravis/shared/gf1/voice/vocctrl.h>
#include <ham/drivers/gravis/shared/gf1/global/dramioad.h>

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

    extern void SetTimer1Handler(const TimerCallback_t& callback, uint8_t ticksPerSecond);
    extern void SetTimer2Handler(const TimerCallback_t& callback, uint8_t ticksPerSecond);


    typedef std::function<void(Midi::Status_t status)> MidiTransmitCallback_t;
    
    extern void SetMidiTransmitCallback(const MidiTransmitCallback_t& callback);


    typedef std::function<void(Midi::Status_t status, Midi::Data_t data)> MidiReceiveCallback_t;

    extern void SetMidiReceiveCallback(const MidiReceiveCallback_t& callback);

    extern void UploadSound(GF1::Global::DramIOAddress_t boardAddress, uint8_t* data, uint32_t length);

    extern void PlayVoice(GF1::Page_t voice,
                          GF1::Global::DramIOAddress_t startLocation,
                          GF1::Global::DramIOAddress_t loopStart,
                          GF1::Global::DramIOAddress_t loopEnd,
                          GF1::Voice::VoiceControl_t voiceControl, bool rollover = false);

    extern void StopVoice(GF1::Page_t voice);
    
    extern void ResumeVoice(GF1::Page_t voice);

    extern void SetVolume(GF1::Page_t voice, GF1::Voice::CurrentVolume_t volume);
    extern void SetLinearVolume(GF1::Page_t voice, uint16_t volume);

    extern void SetPlaybackFrequency(GF1::Page_t voice, uint16_t frequencyInKHz, uint16_t activeVoices);

    extern void SetPan(GF1::Page_t voice, GF1::Voice::PanPosition_t pan);

    extern void ResetVoice(GF1::Page_t voice);
}