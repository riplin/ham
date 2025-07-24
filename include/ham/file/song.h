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

namespace Ham::File
{

#pragma pack(push, 1)

class Song final
{
public:
    static void Destroy(Song* song);
    
    struct Note
    {
        uint8_t Instrument; // 0 = not set. Starts counting at 1. Subtract 1 to get sample index.
        uint16_t Period;    // Amiga period. 0 = not set
        uint16_t Note;      // 0...35, 0xffff = not set.
        uint8_t Effect;     // 0 based. Effect = Parameter = 0 = not set.
        uint8_t Parameter;  // Interpreted based on Effect. Effect = Parameter = 0 = not set.
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
        char Name[23];      // 0 terminated string.
        SampleWidth Width;  // Bit width of sample.
        int8_t FineTune;    // Index in to fine tune table.
        uint8_t Volume;     // Default sample volume (0...64 included). TODO: change scale?

        inline uint8_t WidthInBytes() const { return (Width + 7) >> 3; }
    };

    enum Effect : uint8_t
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
        SetSpeed = 0x0F
    };

    enum SubEffect : uint8_t
    {
        Mask = 0xF0,
        SetFilter = 0x00,
        FinePortaUp = 0x10,
        FinePortaDown = 0x20,
        GlissandoControl = 0x30,
        SetVibratoWaveform = 0x40,
        SetFinetune = 0x50,
        PatternLoop = 0x60,
        SetTremoloWaveForm = 0x70,
        FinePanning = 0x80,
        RetriggerNote = 0x90,
        FineVolumeSlideUp = 0xA0,
        FineVolumeSlideDown = 0xB0,
        CutNote = 0xC0,
        DelayNote = 0xD0,
        PatternDelay = 0xE0,
        InvertLoop = 0xF0
    };

    inline const char* GetName() const {return m_Name; }
    inline uint8_t GetChannelCount() const { return m_ChannelCount; }
    inline uint16_t GetOrderCount() const { return m_OrderCount; }
    inline uint16_t GetOrder(uint16_t order) const { return (order < m_OrderCount) ? m_Orders[order] : 0; }
    inline uint16_t GetPatternCount() const { return m_PatternCount; }
    inline uint16_t GetPatternRowCount(uint16_t pattern) const { return (pattern < m_PatternCount) ? m_Patterns[pattern].RowCount : 0; }
    const Note* GetPatternNotes(uint16_t pattern) const;

    inline uint16_t GetInstrumentCount() const { return m_InstrumentCount; }
    inline const char* GetInstrumentName(uint16_t instrument) const { return (instrument < m_InstrumentCount) ? m_Instruments[instrument].Name : nullptr; }

    inline const Sample* GetSample(uint16_t instrument, uint16_t sample) const 
    {
        return ((instrument < m_InstrumentCount) &&
                (sample < m_Instruments[instrument].SampleCount)) ? 
                &m_Instruments[instrument].Samples[sample] : nullptr;
    }

    inline uint16_t GetPeriod(uint8_t note, uint8_t fineTune) const { return m_TrackerPeriods[m_Tracker].Periods[fineTune * m_TrackerPeriods[m_Tracker].NoteCount + note]; }
    inline const char* GetNoteName(uint8_t note) const { return s_NoteNames[note + m_TrackerPeriods[m_Tracker].NameOffset]; }

protected:
    friend Song* Mod::Load(Has::IAllocator& allocator, const char* filePath);;
    friend Song* S3m::Load(Has::IAllocator& allocator, const char* filePath);;

    static Song* Create(Has::IAllocator& allocator);

    void SetName(const char* name, uint8_t maxLength);

    void SetChannelCount(uint8_t channelCount);
    void SetInstrumentCount(uint16_t instrumentCount);

    void SetInstrumentName(uint16_t instrument, const char* name, uint8_t maxLength);

    void SetSampleCount(uint16_t instrument, uint16_t sampleCount);
    void SetSampleName(uint16_t instrument, uint16_t sample, const char* name, uint8_t maxLength);
    void SetSampleLength(uint16_t instrument, uint16_t sample, uint32_t length, SampleWidth width);
    void SetSampleLoopStart(uint16_t instrument, uint16_t sample, uint32_t loopStart);
    void SetSampleLoopEnd(uint16_t instrument, uint16_t sample, uint32_t loopEnd);
    void SetSampleFineTune(uint16_t instrument, uint16_t sample, uint32_t fineTune);
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
    void SetNote(uint16_t pattern, uint16_t row, uint8_t channel, const Note& note);
    bool ResolveNotes();
    Note* GetNotes(uint16_t pattern);

    void CalculateFastTrackerNotePeriods();

private:
    Song(Has::IAllocator& allocator);
    ~Song();

    struct Instrument
    {
        Sample* Samples;
        char Name[23];          // 0 terminated string.
        uint16_t SampleCount;
    };

    struct Pattern
    {
        Note* Notes;
        uint16_t RowCount;
    };

    enum Tracker : uint8_t
    {
        ProTracker = 0x00,
        TakeTracker = 0x01,
        FastTracker = 0x02
    };

    Has::IAllocator&    m_Allocator;
    char                m_Name[28];
    uint16_t*           m_Orders;
    Instrument*         m_Instruments;
    Pattern*            m_Patterns;
    uint16_t            m_InstrumentCount;
    uint16_t            m_OrderCount;
    uint16_t            m_PatternCount;
    uint8_t             m_ChannelCount;
    Tracker             m_Tracker;

    uint32_t SetNotes(Tracker tracker);

    struct PeriodDescriptor
    {
        uint16_t* Periods;
        uint8_t NoteCount;
        uint8_t NameOffset;
    };

    static PeriodDescriptor m_TrackerPeriods[3];

    static uint16_t s_ProTrackerPeriods[16 * 36];
    static uint16_t s_TakeTrackerPeriods[16 * 60];
    static uint16_t s_FastTrackerPeriods[16 * 96];

    static const char* s_NoteNames[96];
};

#pragma pack(pop)

}