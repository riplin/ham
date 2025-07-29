//Copyright 2025-Present riplin

#pragma once

#include <type_traits>
#include <has/ialloc.h>
#include <has/istream.h>
#include <ham/file/mod.h>
#include <ham/file/s3m.h>

/*
5.1 Effect 0xy (Arpeggio)
5.2 Effect 1xy (Porta Up)
5.3 Effect 2xy (Porta Down)
5.4 Effect 3xy (Porta To Note)
5.5 Effect 4xy (Vibrato)
5.6 Effect 5xy (Porta + Vol Slide)
5.7 Effect 6xy (Vibrato + Vol Slide)
5.8 Effect 7xy (Tremolo)
5.9 Effect 8xy (Pan)                        *
5.10 Effect 9xy (Sample Offset)             *
5.11 Effect Axy (Volume Slide)
5.12 Effect Bxy (Jump To Pattern)           *
5.13 Effect Cxy (Set Volume)                *
5.14 Effect Dxy (Pattern Break)             *
5.15 Effect Fxy (Set Speed)                 *
5.16 Effect E0x (Set Filter)                *
5.17 Effect E1x (Fine Porta Up)             *
5.18 Effect E2x (Fine Porta Down)           *
5.19 Effect E3x (Glissando Control)         *
5.20 Effect E4x (Set Vibrato Waveform)      *
5.21 Effect E5x (Set Finetune)              *
5.22 Effect E6x (Pattern Loop)              *
5.23 Effect E7x (Set Tremolo WaveForm)      *
5.24 Effect E8x (16 Position Panning)       *
5.25 Effect E9x (Retrig Note)
5.26 Effect EAx (Fine Volume Slide Up)      *
5.27 Effect EBx (Fine Volume Slide Down)    *
5.28 Effect ECx (Cut Note)
5.29 Effect EDx (Delay Note)
5.30 Effect EEx (Pattern Delay)             *
5.31 Effect EFx (Invert Loop)               *

* == On Tick

*/

namespace Ham::Music
{

typedef uint8_t Effect_t;
namespace Effect
{
    enum
    {
        Arpeggio = 0x00,
        PortamentoUp = 0x01,
        PortamentoDown = 0x02,
        PortamentoToNote = 0x03,
        Vibrato = 0x04,
        PortamentoAndVolumeSlide = 0x05,
        VibratoAndVolumeSlide = 0x06,
        Tremolo = 0x07,
        Pan = 0x08,
        SampleOffset = 0x09,
        VolumeSlide = 0x0A,
        JumpToPattern = 0x0B,
        SetVolume = 0x0C,
        PatternBreak = 0x0D,
        MoreEffects = 0x0E, // -> SubEffect
        SetSpeed = 0x0F,

        //Scream Tracker 3.x effects
        SetSpeedEx = 0x10,                      // 
        VolumeSlideEx = 0x11,
        PortamentoDownEx = 0x12,
        PortamentoUpEx = 0x13,
        Tremor = 0x14,
        VibratoAndVolumeSlideEx = 0x15,
        PortamentoAndVolumeSlideEx = 0x16,
        RetriggerAndVolumeSlide = 0x17,
        SetTempo = 0x18,
        FineVibrato = 0x19,
        GlobalVolume = 0x1a,
        StereoControl = 0x1b,
    };
}

typedef uint8_t SubEffect_t;
namespace SubEffect
{
    enum
    {
        Mask = 0xF0,
        SetFilter = 0x00,
        FinePortaUp = 0x10,
        FinePortaDown = 0x20,
        GlissandoControl = 0x30,
        SetVibratoWaveform = 0x40,
        SetFinetune = 0x50,
        PatternLoop = 0x60,
        SetTremoloWaveform = 0x70,
        FinePanning = 0x80,
        RetriggerNote = 0x90,
        FineVolumeSlideUp = 0xA0,
        FineVolumeSlideDown = 0xB0,
        CutNote = 0xC0,
        DelayNote = 0xD0,
        PatternDelay = 0xE0,
        InvertLoop = 0xF0
    };
}

typedef uint8_t Volume_t;
namespace Volume
{
    enum
    {
        Min = 0x00,
        Max = 0x40,
        NotSet = 0xFF
    };
}

typedef uint8_t Note_t;
namespace Note
{
    enum
    {
        KeyOff = 132,//Last two entries of s_NotePeriods are 0
        NotSet = 133,//Last two entries of s_NotePeriods are 0
        MiddleC = 8363  //This value is not stored in Note_t, but made sense to put here instead of hardcoded values everywhere.
    };
}

#pragma pack(push, 1)

class Song final
{
public:
    static void Destroy(Song* song);
    
    struct NoteData
    {
        uint8_t Instrument; // 0 = not set. Starts counting at 1. Subtract 1 to get sample index.
        uint16_t Note;      // 0...35, 0xffff = not set.
        Effect_t Effect;    // 0 based. Effect = Parameter = 0 = not set.
        uint8_t Parameter;  // Interpreted based on Effect. Effect = Parameter = 0 = not set.
        uint8_t Volume;     // 0...64, 0xFF = not set.
    };

    enum SampleWidth : uint8_t
    {
        Invalid = 0x00,
        BitWidth8 = 0x08,
        BitWidth16 = 0x10,
    };

    struct Sample
    {
        void* Data;         // Data of sample. Deliberately void.
        uint32_t Length;    // Length in samples minus one.
        uint32_t LoopStart; // Start of loop in samples.
        uint32_t LoopEnd;   // End of loop in samples. Points 1 byte after loop end!
        char Name[28];      // 0 terminated string.
        SampleWidth Width;  // Bit width of sample.
        uint8_t Volume;     // Default sample volume (0...64 included). TODO: change scale?

        inline uint8_t WidthInBytes() const { return (Width + 7) >> 3; }
    };

    inline const char* GetName() const {return m_Name; }
    inline uint8_t GetSpeed() const { return m_Speed; }
    inline uint8_t GetBpm() const { return m_Bpm; }
    inline uint8_t GetVolume() const { return m_Volume; }

    inline uint8_t GetChannelCount() const { return m_ChannelCount; }
    inline uint8_t GetChannelBalance(uint8_t channel) const { return (channel < m_ChannelCount) ? m_Channels[channel].Balance : 0x07; }

    inline uint16_t GetOrderCount() const { return m_OrderCount; }
    inline uint16_t GetOrder(uint16_t order) const { return (order < m_OrderCount) ? m_Orders[order] : 0; }

    inline uint16_t GetPatternCount() const { return m_PatternCount; }
    inline uint16_t GetPatternRowCount(uint16_t pattern) const { return (pattern < m_PatternCount) ? m_Patterns[pattern].RowCount : 0; }
    const NoteData* GetPatternNotes(uint16_t pattern) const;

    inline uint16_t GetInstrumentCount() const { return m_InstrumentCount; }
    inline const char* GetInstrumentName(uint16_t instrument) const { return (instrument < m_InstrumentCount) ? m_Instruments[instrument].Name : nullptr; }
    inline uint16_t GetInstrumentMiddleC(uint16_t instrument) const { return (instrument < m_InstrumentCount) ? m_Instruments[instrument].MiddleC : Note::MiddleC; }

    inline const Sample* GetSample(uint16_t instrument, uint16_t sample) const 
    {
        return ((instrument < m_InstrumentCount) &&
                (sample < m_Instruments[instrument].SampleCount)) ? 
                &m_Instruments[instrument].Samples[sample] : nullptr;
    }

    static uint16_t ConvertFineTuneToPeriod(uint8_t fineTune);
    static inline uint16_t GetPeriod(Note_t note) { return s_NotePeriods[Has::min<Note_t>(note, (sizeof(s_NotePeriods) / sizeof(*s_NotePeriods)) - 1)]; }
protected:
    friend Song* File::Mod::Load(Has::IAllocator& allocator, const char* filePath);;
    friend Song* File::S3m::Load(Has::IAllocator& allocator, const char* filePath);;

    static Song* Create(Has::IAllocator& allocator);

    void SetName(const char* name, uint8_t maxLength);
    void SetSpeed(uint8_t speed);
    void SetBpm(uint8_t bpm);
    void SetVolume(uint8_t volume);

    void SetChannelCount(uint8_t channelCount);
    void SetChannelBalance(uint8_t channel, uint8_t balance);

    void SetInstrumentCount(uint16_t instrumentCount);
    void SetInstrumentName(uint16_t instrument, const char* name, uint8_t maxLength);
    void SetInstrumentMiddleC(uint16_t instrument, uint16_t middleC);

    void SetSampleCount(uint16_t instrument, uint16_t sampleCount);
    void SetSampleName(uint16_t instrument, uint16_t sample, const char* name, uint8_t maxLength);
    void SetSampleLength(uint16_t instrument, uint16_t sample, uint32_t length, SampleWidth width);
    void SetSampleLoopStart(uint16_t instrument, uint16_t sample, uint32_t loopStart);
    void SetSampleLoopEnd(uint16_t instrument, uint16_t sample, uint32_t loopEnd);
    void SetSampleVolume(uint16_t instrument, uint16_t sample, uint8_t volume);
    bool LoadSampleData(uint16_t instrument, uint16_t sample, Has::IStream& stream);

    void SetOrderCount(uint16_t orderCount);
    void SetOrder(uint16_t index, uint16_t order);

    template<typename OrderType>
    bool LoadOrders(uint16_t orderCount, Has::IStream& stream)
    {
        if (sizeof(OrderType) == sizeof(std::remove_reference<decltype(*m_Orders)>::type))
        {
            SetOrderCount(orderCount);
            if (!stream.Read(orderCount * sizeof(OrderType), m_Orders))
                return false;
        }
        else
        {
            OrderType temp = OrderType();
            for (uint16_t index = 0; index < orderCount; ++index)
            {
                if (!stream.Read(1, &temp))
                    return false;
                SetOrder(index, temp);
            }
        }
        return true;
    }

    void SetPatternCount(uint16_t patternCount);
    void SetPatternRowCount(uint16_t pattern, uint16_t rowCount);
    void SetNote(uint16_t pattern, uint16_t row, uint8_t channel, const NoteData& note);
    NoteData* GetNotes(uint16_t pattern);

    static Note_t PeriodToNote(uint16_t period);

private:
    Song(Has::IAllocator& allocator);
    ~Song();

    struct Instrument
    {
        Sample* Samples;
        char Name[28];          // 0 terminated string.
        uint16_t SampleCount;
        uint16_t MiddleC;       // Middle C period.
    };

    struct Pattern
    {
        NoteData* Notes;
        uint16_t RowCount;
    };

    struct Channel
    {
        uint8_t Balance;
    };

    Has::IAllocator&    m_Allocator;
    char                m_Name[28];
    Channel*            m_Channels;
    uint16_t*           m_Orders;
    Instrument*         m_Instruments;
    Pattern*            m_Patterns;
    uint16_t            m_InstrumentCount;
    uint16_t            m_OrderCount;
    uint16_t            m_PatternCount;
    uint8_t             m_ChannelCount;
    uint8_t             m_Speed;
    uint8_t             m_Bpm;
    uint8_t             m_Volume;

    static uint16_t s_FineTuneToPeriod[16];
    static uint16_t s_NotePeriods[134];
};

#pragma pack(pop)

}