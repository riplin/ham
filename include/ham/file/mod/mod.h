//Copyright 2025-Present riplin

#pragma once

#include <has/ialloc.h>

namespace Ham::File
{

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

        inline const char* GetName() { return m_Name; }
        inline uint8_t GetChannelCount() { return m_ChannelCount; }
        inline uint8_t GetOrderCount() { return m_OrderCount; }
        inline uint8_t GetOrder(uint8_t index) { return m_Orders[index]; }
        inline uint8_t GetPatternCount() { return m_PatternCount;}
        inline Note* GetPattern(uint8_t index) { return m_Patterns[index]; }
        
        inline const char* GetNoteName(uint8_t note) { return s_Notes[note]; }

        inline uint8_t GetSampleCount() { return 31; }
        inline const char* GetSampleName(uint8_t index) { return m_Samples[index].Name; }
        inline uint16_t GetSampleLength(uint8_t index) { return m_Samples[index].Length; }
        inline uint8_t GetSampleFineTune(uint8_t index) { return m_Samples[index].FineTune; }
        inline uint8_t GetSampleVolume(uint8_t index) { return m_Samples[index].Volume; }
        inline uint16_t GetSampleLoopStart(uint8_t index) { return m_Samples[index].LoopStart; }
        inline uint16_t GetSampleLoopEnd(uint8_t index) { return m_Samples[index].LoopEnd; }
    private:
        Mod(Has::IAllocator& allocator);
        ~Mod();
        static Mod* Create(Has::IAllocator& allocator);

        struct Sample
        {
            char Name[23];
            uint16_t Length;
            int8_t FineTune;
            uint8_t Volume;
            uint16_t LoopStart;
            uint16_t LoopEnd;
        };

        Has::IAllocator& m_Allocator;
        char m_Name[21];
        Sample m_Samples[31];
        uint8_t m_ChannelCount;
        uint8_t m_OrderCount;
        uint8_t m_Orders[128];
        uint8_t m_PatternCount;
        Note* m_Patterns[256];
        uint8_t* m_SampleData[31];

        static uint16_t s_Periods[16][36];
        static const char* s_Notes[36];
    };

#pragma pack(pop)

}