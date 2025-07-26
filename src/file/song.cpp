//Copyright 2025-Present riplin

#include <new>
#include <string.h>
#include <ham/file/song.h>
#include <has/testing/log.h>

namespace Ham::File
{

Song::Song(Has::IAllocator& allocator)
    : m_Allocator(allocator)
    , m_Channels(nullptr)
    , m_Orders(nullptr)
    , m_Instruments(nullptr)
    , m_Patterns(nullptr)
    , m_InstrumentCount(0)
    , m_OrderCount(0)
    , m_PatternCount(0)
    , m_ChannelCount(0)
    , m_Speed(0)
    , m_Bpm(0)
    , m_Volume(0)
{
    memset(m_Name, 0, sizeof(m_Name));
}

Song::~Song()
{
    LOG("Song", "Destructor called");
    SetPatternCount(0);
    SetInstrumentCount(0);
    SetOrderCount(0);
}

Song* Song::Create(Has::IAllocator& allocator)
{
    void* ptr = allocator.Allocate(sizeof(Song));
    if (ptr == nullptr)
    {
        LOG("Song", "Could not allocate memory for song");
        return nullptr;
    }

    Song* song = ::new(ptr) Song(allocator);
    LOG("Song", "Song constructed");

    return song;
}

void Song::Destroy(Song* song)
{
    if (song != nullptr)
    {
        Has::IAllocator& allocator = song->m_Allocator;
        song->~Song();
        allocator.Free(song);
        LOG("Song", "Song destroyed");
    }
}

void Song::SetChannelCount(uint8_t channelCount)
{
    using namespace Has;

    if (channelCount != m_ChannelCount)
    {
        auto* oldChannels = m_Channels;
        auto oldChannelCount = m_ChannelCount;
        m_ChannelCount = channelCount;
        m_Channels = nullptr;

        if (m_ChannelCount != 0)
        {
            m_Channels = m_Allocator.AllocateAs<std::remove_reference<decltype(*m_Channels)>::type>(sizeof(*m_Channels) * m_ChannelCount);
            if (m_Channels == nullptr)
            {
                m_ChannelCount = 0;

                LOG("Song", "Could not allocate memory for %i channels", channelCount);
                return;
            }
            memset(m_Channels, 0, sizeof(*m_Channels) * m_ChannelCount);
        }

        if ((oldChannels != nullptr) && (m_Channels != nullptr))
        {
            uint16_t copyCount = min<uint16_t>(oldChannelCount, channelCount);
            memcpy(m_Channels, oldChannels, copyCount * sizeof(m_Channels[0]));
        }

        if (oldChannels != nullptr)
        {
            m_Allocator.Free(oldChannels);
        }



        //TODO: Go through the patterns and resize.

        LOG("Song", "Channel count set to: %i", m_ChannelCount);
    }
}

void Song::SetChannelBalance(uint8_t channel, uint8_t balance)
{
    if (channel < m_ChannelCount)
    {
        m_Channels[channel].Balance = balance;

        LOG("Song", "Channel %i balance set to %i", channel, balance);
    }
}

void Song::SetName(const char* name, uint8_t maxLength)
{
    using namespace Has;

    memset(m_Name, 0, sizeof(m_Name));
    maxLength = min<uint8_t>(maxLength, sizeof(m_Name) - 1);

    uint8_t idx = 0;
    while ((idx < maxLength) && name[idx] != '\0')
    {
        m_Name[idx] = name[idx];
        ++idx;
    }

    LOG("Song", "Name set to: [%s]", m_Name);
}

void Song::SetSpeed(uint8_t speed)
{
    m_Speed = speed;
    LOG("Song", "Speed set to %i", m_Speed);
}

void Song::SetBpm(uint8_t bpm)
{
    m_Bpm = bpm;
    LOG("Song", "Beats per minute set to %i", m_Bpm);
}

void Song::SetVolume(uint8_t volume)
{
    m_Volume = volume;
    LOG("Song", "Volume set to %i", m_Volume);
}

void Song::SetInstrumentCount(uint16_t instrumentCount)
{
    using namespace Has;

    if (instrumentCount != m_InstrumentCount)
    {
        auto* oldInstruments = m_Instruments;
        auto oldInstrumentCount = m_InstrumentCount;

        m_InstrumentCount = instrumentCount;
        m_Instruments = nullptr;

        if (m_InstrumentCount != 0)
        {
            m_Instruments = m_Allocator.AllocateAs<std::remove_reference<decltype(*m_Instruments)>::type>(sizeof(*m_Instruments) * m_InstrumentCount);
            if (m_Instruments == nullptr)
            {
                m_InstrumentCount = 0;

                LOG("Song", "Could not allocate memory for %i instruments", instrumentCount);
                return;
            }

            LOG("Song", "Clearing %i bytes of instrument data", sizeof(*m_Instruments) * m_InstrumentCount);
            memset(m_Instruments, 0, sizeof(*m_Instruments) * m_InstrumentCount);
        }

        uint16_t copyCount = min<uint16_t>(oldInstrumentCount, m_InstrumentCount);
        LOG("Song", "Copying %i instruments", copyCount);
        for (uint16_t index = 0; index < oldInstrumentCount; ++index)
        {
            if (index < copyCount)
            {
                m_Instruments[index] = oldInstruments[index];
            }
            else
            {
                SetSampleCount(index, 0);
            }
        }

        if (oldInstruments != nullptr)
        {
            m_Allocator.Free(oldInstruments);
        }

        LOG("Song", "Instrument count set to %i", m_InstrumentCount);
    }
}

void Song::SetInstrumentName(uint16_t instrument, const char* name, uint8_t maxLength)
{
    using namespace Has;

    if (instrument < m_InstrumentCount)
    {
        memset(m_Instruments[instrument].Name, 0, sizeof(m_Instruments[instrument].Name));
        maxLength = min<uint8_t>(maxLength, sizeof(m_Instruments[instrument].Name) - 1);

        uint8_t idx = 0;
        while ((idx < maxLength) && name[idx] != '\0')
        {
            m_Instruments[instrument].Name[idx] = name[idx];
            ++idx;
        }

        LOG("Song", "Instrument %i name set to [%s]", instrument, m_Instruments[instrument].Name);
    }
}

void Song::SetInstrumentMiddleC(uint16_t instrument, uint16_t middleC)
{
    if (instrument < m_InstrumentCount)
    {
        m_Instruments[instrument].MiddleC = middleC;

        LOG("Song", "Instrument %i middle C set to %i", instrument, middleC);
    }
}

void Song::SetSampleCount(uint16_t instrument, uint16_t sampleCount)
{
    using namespace Has;

    if ((instrument < m_InstrumentCount) && (sampleCount != m_Instruments[instrument].SampleCount))
    {
        auto* oldSamples = m_Instruments[instrument].Samples;
        auto oldSampleCount = m_Instruments[instrument].SampleCount;

        m_Instruments[instrument].SampleCount = sampleCount;
        m_Instruments[instrument].Samples = nullptr;

        if (m_Instruments[instrument].SampleCount != 0)
        {
            m_Instruments[instrument].Samples = m_Allocator.AllocateAs<std::remove_reference<decltype(*m_Instruments[instrument].Samples)>::type>(sizeof(*m_Instruments[instrument].Samples) * m_Instruments[instrument].SampleCount);
            if (m_Instruments[instrument].Samples == nullptr)
            {
                m_Instruments[instrument].SampleCount = 0;

                LOG("Song", "Could not allocate memory for %i samples", sampleCount);
                return;
            }
            memset(m_Instruments[instrument].Samples, 0, sizeof(*m_Instruments[instrument].Samples) * m_Instruments[instrument].SampleCount);
        }

        uint16_t copyCount = min<uint16_t>(oldSampleCount, m_Instruments[instrument].SampleCount);
        LOG("Song", "Copying %i samples", copyCount);
        for (uint16_t index = 0; index < oldSampleCount; ++index)
        {
            if (index < copyCount)
            {
                m_Instruments[instrument].Samples[index] = oldSamples[index];
            }
            else
            {
                m_Allocator.Free(oldSamples[index].Data);
            }
        }

        if (oldSamples != nullptr)
        {
            m_Allocator.Free(oldSamples);
        }

        LOG("Song", "Instrument %i sample count set to %i", instrument, m_Instruments[instrument].SampleCount);
    }
}

void Song::SetSampleName(uint16_t instrument, uint16_t sample, const char* name, uint8_t maxLength)
{
    using namespace Has;

    if ((instrument < m_InstrumentCount) && (sample < m_Instruments[instrument].SampleCount))
    {
        memset(m_Instruments[instrument].Samples[sample].Name, 0, sizeof(m_Instruments[instrument].Samples[sample].Name));
        maxLength = min<uint8_t>(maxLength, sizeof(m_Instruments[instrument].Samples[sample].Name) - 1);

        uint8_t idx = 0;
        while ((idx < maxLength) && name[idx] != '\0')
        {
            m_Instruments[instrument].Samples[sample].Name[idx] = name[idx];
            ++idx;
        }

        LOG("Song", "Instrument %i sample %i name set to [%s]", instrument, sample, m_Instruments[instrument].Samples[sample].Name);
    }
}

void Song::SetSampleLength(uint16_t instrument, uint16_t sample, uint32_t length, SampleWidth width)
{
    if ((instrument < m_InstrumentCount) && (sample < m_Instruments[instrument].SampleCount))
    {
        if (m_Instruments[instrument].Samples[sample].Data != nullptr)
        {
            m_Allocator.Free(m_Instruments[instrument].Samples[sample].Data);
            m_Instruments[instrument].Samples[sample].Data = nullptr;
            m_Instruments[instrument].Samples[sample].Length = 0;
        }

        m_Instruments[instrument].Samples[sample].Length = length;
        m_Instruments[instrument].Samples[sample].Width = width;

        if(m_Instruments[instrument].Samples[sample].Length > 0)
        {
            uint32_t bytes = (m_Instruments[instrument].Samples[sample].Length + 1) * (m_Instruments[instrument].Samples[sample].Width >> 3);
            m_Instruments[instrument].Samples[sample].Data = m_Allocator.Allocate(bytes);
            if (m_Instruments[instrument].Samples[sample].Data == nullptr)
            {
                m_Instruments[instrument].Samples[sample].Length = 0;
                m_Instruments[instrument].Samples[sample].Width = SampleWidth::Invalid;

                LOG("Song", "Could not allocate memory for %i bytes of sample data at %i bits", length, width);
                return;
            }
            memset(m_Instruments[instrument].Samples[sample].Data, 0, bytes);
        }

        LOG("Song", "Instrument %i sample %i data length set to %i, with bit width %i", instrument, sample, m_Instruments[instrument].Samples[sample].Length, m_Instruments[instrument].Samples[sample].Width);
    }
}

void Song::SetSampleLoopStart(uint16_t instrument, uint16_t sample, uint32_t loopStart)
{
    if ((instrument < m_InstrumentCount) && (sample < m_Instruments[instrument].SampleCount))
    {
        m_Instruments[instrument].Samples[sample].LoopStart = loopStart;

        LOG("Song", "Instrument %i sample %i loop start set to %i", instrument, sample, loopStart);
    }
}

void Song::SetSampleLoopEnd(uint16_t instrument, uint16_t sample, uint32_t loopEnd)
{
    if ((instrument < m_InstrumentCount) && (sample < m_Instruments[instrument].SampleCount))
    {
        if (loopEnd > m_Instruments[instrument].Samples[sample].Length)
            loopEnd = m_Instruments[instrument].Samples[sample].Length;

        m_Instruments[instrument].Samples[sample].LoopEnd = loopEnd;

        LOG("Song", "Instrument %i sample %i loop end set to %i", instrument, sample, loopEnd);
    }
}

void Song::SetSampleVolume(uint16_t instrument, uint16_t sample, uint8_t volume)
{
    if ((instrument < m_InstrumentCount) && (sample < m_Instruments[instrument].SampleCount))
    {
        m_Instruments[instrument].Samples[sample].Volume = volume;

        LOG("Song", "Instrument %i sample %i volume set to %i", instrument, sample, volume);
    }
}

bool Song::LoadSampleData(uint16_t instrument, uint16_t sample, Has::IStream& stream)
{
    if ((instrument < m_InstrumentCount) && (sample < m_Instruments[instrument].SampleCount))
    {
        if (!stream.Read(m_Instruments[instrument].Samples[sample].Length * m_Instruments[instrument].Samples[sample].WidthInBytes(), m_Instruments[instrument].Samples[sample].Data))
        {
            LOG("Song", "Failed to load %i bytes of sample data", m_Instruments[instrument].Samples[sample].Length * m_Instruments[instrument].Samples[sample].WidthInBytes());
            return false;
        }

        void* loopStart = static_cast<uint8_t*>(m_Instruments[instrument].Samples[sample].Data) + m_Instruments[instrument].Samples[sample].LoopStart * m_Instruments[instrument].Samples[sample].WidthInBytes();
        void* loopEnd = static_cast<uint8_t*>(m_Instruments[instrument].Samples[sample].Data) + m_Instruments[instrument].Samples[sample].LoopEnd * m_Instruments[instrument].Samples[sample].WidthInBytes();
        void* end = static_cast<uint8_t*>(m_Instruments[instrument].Samples[sample].Data) + m_Instruments[instrument].Samples[sample].Length * m_Instruments[instrument].Samples[sample].WidthInBytes();

        // Copy one sample to the end to prevent clicking.
        if (m_Instruments[instrument].Samples[sample].LoopEnd != 0)
        {
            memcpy(loopEnd, loopStart, m_Instruments[instrument].Samples[sample].WidthInBytes());
        }
        else
        {
            memset(end, 0, m_Instruments[instrument].Samples[sample].WidthInBytes());
        }

        LOG("Song", "Loaded %i bytes of sample data", m_Instruments[instrument].Samples[sample].Length * m_Instruments[instrument].Samples[sample].WidthInBytes());
        return true;
    }
    return false;
}

void Song::SetOrderCount(uint16_t orderCount)
{
    using namespace Has;

    if (orderCount != m_OrderCount)
    {
        auto* oldOrders = m_Orders;
        auto oldOrderCount = m_OrderCount;
        m_OrderCount = orderCount;
        m_Orders = nullptr;

        if (m_OrderCount != 0)
        {
            m_OrderCount = orderCount;
            m_Orders = m_Allocator.AllocateAs<std::remove_reference<decltype(*m_Orders)>::type>(sizeof(*m_Orders) * m_OrderCount);
            if (m_Orders == nullptr)
            {
                m_OrderCount = 0;

                LOG("Song", "Could not allocate memory for %i pattern indices", orderCount);
                return;
            }
            memset(m_Orders, 0, sizeof(*m_Orders) * m_OrderCount);
        }

        if ((oldOrders != nullptr) && (m_Orders != nullptr))
        {
            uint16_t copyCount = min<uint16_t>(oldOrderCount, orderCount);
            memcpy(m_Orders, oldOrders, copyCount * sizeof(m_Orders[0]));
        }

        if (oldOrders != nullptr)
        {
            m_Allocator.Free(oldOrders);
        }

        LOG("Song", "Order count set to %i", m_OrderCount);
    }
}

void Song::SetOrder(uint16_t index, uint16_t order)
{
    if (index < m_OrderCount)
    {
        m_Orders[index] = order;

        LOG("Song", "Order %i set to %i", index, order);
    }
}

void Song::SetPatternCount(uint16_t patternCount)
{
    using namespace Has;

    if (patternCount != m_PatternCount)
    {
        auto* oldPatterns = m_Patterns;
        auto oldPatternCount = m_PatternCount;

        m_Patterns = nullptr;
        m_PatternCount = patternCount;
        
        if (m_PatternCount != 0)
        {
            m_Patterns = m_Allocator.AllocateAs<Pattern>(sizeof(Pattern) * m_PatternCount);
            if (m_Patterns == nullptr)
            {
                m_PatternCount = 0;

                LOG("Song", "Could not allocate memory for %i patterns", patternCount);
                return;
            }
            memset(m_Patterns, 0, sizeof(Pattern) * m_PatternCount);
        }

        uint16_t copyCount = min<uint16_t>(oldPatternCount, m_PatternCount);
        LOG("Song", "Copying %i patterns", copyCount);
        for (uint16_t patternIndex = 0; patternIndex < oldPatternCount; ++patternIndex)
        {
            if (patternIndex < copyCount)
            {
                m_Patterns[patternIndex] = oldPatterns[patternIndex];
            }
            else
            {
                m_Allocator.Free(oldPatterns[patternIndex].Notes);
            }
        }

        if (oldPatterns != nullptr)
        {
            m_Allocator.Free(oldPatterns);
        }

        LOG("Song", "Pattern count set to %i", m_PatternCount);
    }
}

void Song::SetPatternRowCount(uint16_t pattern, uint16_t rowCount)
{
    using namespace Has;

    if ((pattern < m_PatternCount) && (m_Patterns[pattern].RowCount != rowCount))
    {
        auto* oldNotes = m_Patterns[pattern].Notes;
        auto oldRowCount = m_Patterns[pattern].RowCount;
        
        m_Patterns[pattern].Notes = nullptr;
        m_Patterns[pattern].RowCount = rowCount;

        if (m_Patterns[pattern].RowCount > 0)
        {
            m_Patterns[pattern].Notes = m_Allocator.AllocateAs<NoteData>(sizeof(NoteData) * m_Patterns[pattern].RowCount * m_ChannelCount);
            if (m_Patterns[pattern].Notes == nullptr)
            {
                m_Patterns[pattern].RowCount = 0;

                LOG("Song", "Could not allocate memory for %i notes", m_Patterns[pattern].RowCount * m_ChannelCount);
                return;
            }
            memset(m_Patterns[pattern].Notes, 0, sizeof(NoteData) * m_Patterns[pattern].RowCount * m_ChannelCount);
        }

        if ((oldNotes != nullptr) && (m_Patterns[pattern].Notes != nullptr))
        {
            uint16_t copyCount = min<uint16_t>(oldRowCount, m_Patterns[pattern].RowCount);
            LOG("Song", "Copying %i rows", copyCount);
            memcpy(m_Patterns[pattern].Notes, oldNotes, sizeof(NoteData) * copyCount * m_ChannelCount);
        }

        if (oldNotes != nullptr)
        {
            m_Allocator.Free(oldNotes);
        }

        LOG("Song", "Pattern %i row count set to %i", pattern, rowCount);
    }
}

const Song::NoteData* Song::GetPatternNotes(uint16_t pattern) const 
{
    if (pattern < m_PatternCount)
    {
        return m_Patterns[pattern].Notes;
    }
    return nullptr;
}

Song::NoteData* Song::GetNotes(uint16_t pattern)
{
    if (pattern < m_PatternCount)
    {
        return m_Patterns[pattern].Notes;
    }
    return nullptr;
}

void Song::SetNote(uint16_t pattern, uint16_t row, uint8_t channel, const NoteData& note)
{
    if ((pattern < m_PatternCount) && (row < m_Patterns[pattern].RowCount))
    {
        m_Patterns[pattern].Notes[row * m_ChannelCount + channel] = note;
    }
}

uint16_t Song::s_FineTuneToPeriod[16] =
{
    8363,
    8413,
    8463,
    8529,
    8581,
    8651,
    8723,
    8757,
    7895,
    7941,
    7985,
    8046,
    8107,
    8169,
    8232,
    8280
};

uint16_t Song::s_NotePeriods[134] =
{
    //  C     C#      D     D#      E      F     F#      G     G#      A     A#      B
    27392, 25856, 24384, 23040, 21696, 20480, 19328, 18240, 17216, 16256, 15360, 14496, // 0
    13696, 12928, 12192, 11520, 10848, 10240,  9664,  9120,  8608,  8128,  7680,  7248, // 1
     6848,  6464,  6096,  5760,  5424,  5120,  4832,  4560,  4304,  4064,  3840,  3624, // 2
     3424,  3232,  3048,  2880,  2712,  2560,  2416,  2280,  2152,  2032,  1920,  1812, // 3
     1712,  1616,  1524,  1440,  1356,  1280,  1208,  1140,  1076,  1016,   960,   906, // 4
      856,   808,   762,   720,   678,   640,   604,   570,   538,   508,   480,   453, // 5
      428,   404,   381,   360,   339,   320,   302,   285,   269,   254,   240,   226, // 6
      214,   202,   190,   180,   170,   160,   151,   143,   135,   127,   120,   113, // 7
      107,   101,    95,    90,    85,    80,    75,    71,    67,    63,    60,    56, // 8
       53,    50,    47,    45,    42,    40,    37,    35,    33,    31,    30,    28, // 9
       26,    25,    23,    22,    21,    20,    18,    17,    16,    15,    15,    14, // 10
        0,     0    // <- KeyOff and NoNote
};

uint16_t Song::ConvertFineTuneToPeriod(uint8_t fineTune)
{
    if (fineTune > 15)
        fineTune = 0;
    return s_FineTuneToPeriod[fineTune];
}

Note_t Song::PeriodToNote(uint16_t period)
{
    for (Note_t note = 0; note < sizeof(s_NotePeriods) / sizeof(*s_NotePeriods); ++note)
    {
        if (period >= s_NotePeriods[note])
        {
            return note;
        }
    }
    return Note::NotSet;
}

}