//Copyright 2025-Present riplin

#pragma once

#include <has/ialloc.h>

namespace Ham::File
{

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

#pragma pack(push, 1)

    class Mod
    {
    public:
        static Mod* Load(Has::IAllocator& allocator, const char* fileName);
        static void Destroy(Mod* mod);
        
        struct Note
        {
            uint8_t Sample;
            uint16_t Period;//TODO: Convert to note.
            uint16_t Note;//0...35, 0xffff = invalid.
            uint8_t Effect;
            uint8_t Parameter;
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

        inline const char* GetName() const { return m_Name; }
        inline uint8_t GetChannelCount() const { return m_ChannelCount; }
        inline uint8_t GetOrderCount() const { return m_OrderCount; }
        inline uint8_t GetOrder(uint8_t index) const { return m_Orders[index]; }
        inline uint8_t GetPatternCount() const { return m_PatternCount;}
        inline Note* GetPattern(uint8_t index) const { return m_Patterns[index]; }
        
        inline const char* GetNoteName(uint8_t note) const { return s_NoteNames[note + m_TrackerPeriods[m_Tracker].NameOffset]; }

        inline uint8_t GetSampleCount() const { return 31; }
        inline const char* GetSampleName(uint8_t index) const { return m_Samples[index].Name; }
        inline uint16_t GetSampleLength(uint8_t index) const { return m_Samples[index].Length; }
        inline uint8_t GetSampleFineTune(uint8_t index) const { return m_Samples[index].FineTune; }
        inline uint8_t GetSampleVolume(uint8_t index) const { return m_Samples[index].Volume; }
        inline uint16_t GetSampleLoopStart(uint8_t index) const { return m_Samples[index].LoopStart; }
        inline uint16_t GetSampleLoopEnd(uint8_t index) const { return m_Samples[index].LoopEnd; }
        inline uint8_t* GetSampleData(uint8_t index) const { return m_Samples[index].Data; }
        inline uint32_t GetSampleAddress(uint8_t index) const { return m_Samples[index].Address; }
        inline void SetSampleAddress(uint8_t index, uint32_t address) { m_Samples[index].Address = address; }

        inline uint16_t GetPeriod(uint8_t note, uint8_t fineTune) { return m_TrackerPeriods[m_Tracker].Periods[fineTune * m_TrackerPeriods[m_Tracker].NoteCount + note]; }

    private:
        Mod(Has::IAllocator& allocator);
        ~Mod();
        static Mod* Create(Has::IAllocator& allocator);

        void CalculateFastTrackerNotePeriods();

        struct Sample
        {
            char Name[23];
            uint16_t Length;
            int8_t FineTune;
            uint8_t Volume;
            uint16_t LoopStart;
            uint16_t LoopEnd;
            uint32_t Address;
            uint8_t* Data;
        };

        Has::IAllocator& m_Allocator;
        char m_Name[21];
        Sample m_Samples[31];
        uint8_t m_ChannelCount;
        uint8_t m_OrderCount;
        uint8_t m_Orders[128];
        uint8_t m_PatternCount;
        Note* m_Patterns[256];

        enum Tracker : uint8_t
        {
            ProTracker = 0x00,
            TakeTracker = 0x01,
            FastTracker = 0x02
        };

        void FindNotes();
        uint32_t SetNotes(Tracker tracker);

        Tracker m_Tracker;

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