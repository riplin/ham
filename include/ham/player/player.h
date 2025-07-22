//Copyright 2025-Present riplin

#pragma once

#include <functional>
#include <has/types.h>
#include <has/ialloc.h>
#include <ham/file/mod/mod.h>
#include <ham/drivers/gravis/shared/gf1/global/dramioad.h>

namespace Ham::Player
{

class Player
{
public:

    typedef std::function<void(uint8_t RefreshRateInHz)> SetRefreshRate_t;

    inline Player(Ham::File::Mod* modFile, const SetRefreshRate_t& refreshRateCallback)
        : m_RefreshRateCallback(refreshRateCallback)
        , m_Module(modFile)
    {
        using namespace Ham::Gravis::Shared;
        //TODO: query the card for number of banks.
        m_MemoryRemaining[0] = GF1::Global::DramIOAddress::BankSize - 32;
        m_MemoryRemaining[1] = GF1::Global::DramIOAddress::BankSize;
        m_MemoryRemaining[2] = GF1::Global::DramIOAddress::BankSize;
        m_MemoryRemaining[3] = GF1::Global::DramIOAddress::BankSize;

        Reset();
        UploadSamples();
    }

    inline void Play() { Reset(); m_State = State::Playing; }

    void Tick();

    void Pause();

    void Resume();

    inline bool IsPaused() const { return m_State == State::Paused; }

    inline void Stop() { Reset(); }

    inline uint8_t GetSpeed() const { return m_Speed; }
    inline uint8_t GetBpm() const { return m_Bpm; }

    inline uint8_t GetCurrentTick() const { return m_CurrentTick; }
    inline uint8_t GetCurrentRow() const { return m_CurrentRow; }
    inline uint8_t GetCurrentOrderIndex() const { return m_CurrentOrderIndex; }

    inline uint8_t GetChannelSample(uint8_t channel) const { return m_Channels[channel].Sample; }
    inline uint8_t GetChannelVolume(uint8_t channel) const { return m_Channels[channel].Volume; }
    inline uint16_t GetChannelPeriod(uint8_t channel) const { return m_Channels[channel].Period; }
    inline uint8_t GetChannelBalance(uint8_t channel) const { return m_Channels[channel].Balance; }

    void ToggleChannelMute(uint8_t channel);
    inline bool IsChannelMuted(uint8_t channel) const { return m_Channels[channel].Mute; }

private:

    void Reset();
    void UploadSamples();

    uint32_t AllocateCardMemory(uint32_t size);

    void HandleTick0(uint8_t channel);
    void HandleTickX(uint8_t channel);

    int16_t ProcessArpeggio(uint8_t channel, uint8_t parameter);
    bool ProcessPortamentoToNote(uint8_t channel);
    void ProcessVolumeSlide(uint8_t channel, uint8_t parameter);
    int16_t ProcessVibrato(uint8_t channel);
    int16_t ProcessTremolo(uint8_t channel);

    void ProcessVolume(uint8_t channel, int16_t volumeDelta);

    void CalculateNextValues();

    enum WaveType : uint8_t
    {
        Type = 0x03,
        Sine = 0x00,
        SawTooth = 0x01,
        Square = 0x02,
        Random = 0x03,
        DontRetrigger = 0x04
    };

    struct Channel
    {
        uint32_t SampleBegin;
        uint32_t SampleLoopStart;
        uint32_t SampleLoopEnd;
        int16_t Period;
        uint16_t PortaTarget;
        int16_t Volume;
        int16_t VolumeTarget;
        int16_t VolumeDelta;
        uint16_t Note;
        uint16_t PortaSpeed;
        uint8_t SampleOffset;
        uint8_t FineTune;
        uint8_t Balance;
        uint8_t Parameter;
        uint8_t Sample;
        uint8_t LoopTarget;
        uint8_t LoopCounter;
        uint8_t VibratoDepth;
        uint8_t VibratoSpeed;
        int8_t VibratoPosition;
        WaveType VibratoWaveType;
        uint8_t TremoloDepth;
        uint8_t TremoloSpeed;
        int8_t TremoloPosition;
        WaveType TremoloWaveType;
        bool Mute;
    };

    Channel m_Channels[32];
    SetRefreshRate_t m_RefreshRateCallback;
    Ham::File::Mod* m_Module;

    enum State : uint8_t
    {
        Stopped,
        Paused,
        Playing,
    };
    
    State m_State;

    uint8_t m_Speed;
    uint8_t m_Bpm;

    uint8_t m_PatternDelay;

    uint8_t m_CurrentTick;
    uint8_t m_CurrentRow;
    uint8_t m_CurrentOrderIndex;

    uint8_t m_NextSpeed;
    uint8_t m_NextBpm;
    uint8_t m_NextTick;
    uint8_t m_NextRow;
    uint8_t m_NextOrderIndex;

    uint32_t m_MemoryRemaining[4];

    static uint16_t s_Volume[65];
    static uint8_t s_SineTable[32];
};

}