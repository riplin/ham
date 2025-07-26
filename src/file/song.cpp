//Copyright 2025-Present riplin

#include <new>
#include <math.h>
#include <string.h>
#include <ham/file/song.h>
#include <has/testing/log.h>

namespace Ham::File
{

Song::Song(Has::IAllocator& allocator)
    : m_Allocator(allocator)
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
    CalculateFastTrackerNotePeriods();
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
    m_ChannelCount = channelCount;
    //TODO: Go through the patterns and resize.

    LOG("Song", "Channel count set to: %i", m_ChannelCount);
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

    LOG("Song", "Name set to: %s", m_Name);
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

        LOG("Song", "Instrument %i name set to %s", instrument, m_Instruments[instrument].Name);
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

        LOG("Song", "Instrument %i sample %i name set to %s", instrument, sample, m_Instruments[instrument].Samples[sample].Name);
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

void Song::SetSampleFineTune(uint16_t instrument, uint16_t sample, uint32_t fineTune)
{
    if ((instrument < m_InstrumentCount) && (sample < m_Instruments[instrument].SampleCount))
    {
        m_Instruments[instrument].Samples[sample].FineTune = fineTune;

        LOG("Song", "Instrument %i sample %i fine tune set to %i", instrument, sample, fineTune);
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
            m_Patterns[pattern].Notes = m_Allocator.AllocateAs<Note>(sizeof(Note) * m_Patterns[pattern].RowCount * m_ChannelCount);
            if (m_Patterns[pattern].Notes == nullptr)
            {
                m_Patterns[pattern].RowCount = 0;

                LOG("Song", "Could not allocate memory for %i notes", m_Patterns[pattern].RowCount * m_ChannelCount);
                return;
            }
            memset(m_Patterns[pattern].Notes, 0, sizeof(Note) * m_Patterns[pattern].RowCount * m_ChannelCount);
        }

        if ((oldNotes != nullptr) && (m_Patterns[pattern].Notes != nullptr))
        {
            uint16_t copyCount = min<uint16_t>(oldRowCount, m_Patterns[pattern].RowCount);
            LOG("Song", "Copying %i rows", copyCount);
            memcpy(m_Patterns[pattern].Notes, oldNotes, sizeof(Note) * copyCount * m_ChannelCount);
        }

        if (oldNotes != nullptr)
        {
            m_Allocator.Free(oldNotes);
        }

        LOG("Song", "Pattern %i row count set to %i", pattern, rowCount);
    }
}

const Song::Note* Song::GetPatternNotes(uint16_t pattern) const 
{
    if (pattern < m_PatternCount)
    {
        return m_Patterns[pattern].Notes;
    }
    return nullptr;
}

Song::Note* Song::GetNotes(uint16_t pattern)
{
    if (pattern < m_PatternCount)
    {
        return m_Patterns[pattern].Notes;
    }
    return nullptr;
}

void Song::SetNote(uint16_t pattern, uint16_t row, uint8_t channel, const Note& note)
{
    if ((pattern < m_PatternCount) && (row < m_Patterns[pattern].RowCount))
    {
        m_Patterns[pattern].Notes[row * m_ChannelCount + channel] = note;
    }
}

uint32_t Song::SetNotes(Tracker tracker)
{
    uint32_t errors = 0;
    for (uint8_t pattern = 0; pattern < m_PatternCount; ++pattern)
    {
        Note* notes = GetNotes(pattern);

        for (uint16_t rowNote = 0; rowNote < m_ChannelCount * GetPatternRowCount(pattern); ++rowNote)
        {
            if (notes[rowNote].Period != 0)
            {
                for (uint8_t note = 0; note < m_TrackerPeriods[tracker].NoteCount; ++note)
                {
                    if ((notes[rowNote].Period > (m_TrackerPeriods[tracker].Periods[note] - 2)) &&
                        (notes[rowNote].Period < (m_TrackerPeriods[tracker].Periods[note] + 2)))
                    {
                        notes[rowNote].Note = note;
                        break;
                    }
                }
                if (notes[rowNote].Note == 0xFFFF)
                    ++errors;
            }
        }
    }
    return errors;
}

bool Song::ResolveNotes()
{
    Tracker bestTracker = Tracker::ProTracker;
    uint32_t errors = 0xffffffff;
    for (uint8_t tracker = Tracker::ProTracker; tracker <= Tracker::FastTracker; ++tracker)
    {
        uint32_t e = SetNotes(Tracker(tracker));
        LOG("Song", "Tracker: %i, errors: %i", tracker, e);
        if (e < errors)
        {
            bestTracker = Tracker(tracker);
            errors = e;
        }
    }

    LOG("Song", "Selected tracker %i periods", bestTracker);
    SetNotes(bestTracker);
    m_Tracker = bestTracker;
    return errors < 100;
}

static uint16_t s_FastTrackerPeriod0[96] =
{
    6848, 6464, 6096, 5760, 5424, 5120, 4832, 4560, 4304, 4064, 3840, 3624,
    3424, 3232, 3048, 2880, 2712, 2560, 2416, 2280, 2152, 2032, 1920, 1812,
    1712, 1616, 1524, 1440, 1356, 1280, 1208, 1140, 1076, 1016,  960,  906,
     856,  808,  762,  720,  678,  640,  604,  570,  538,  508,  480,  453,
     428,  404,  381,  360,  339,  320,  302,  285,  269,  254,  240,  226,
     214,  202,  190,  180,  170,  160,  151,  143,  135,  127,  120,  113,
     107,  101,   95,   90,   85,   80,   75,   71,   67,   63,   60,   56,
      53,   50,   47,   45,   42,   40,   37,   35,   33,   31,   30,   28
};

void Song::CalculateFastTrackerNotePeriods()
{
    if (s_FastTrackerPeriods[0] == 0)
    {
        for (int8_t fineTune = 0; fineTune < 16; ++fineTune)
        {
            for (int8_t note = 0; note < int8_t(sizeof(s_FastTrackerPeriod0) / sizeof(uint16_t)); ++note)
            {
                int8_t signedFineTune = fineTune > 7 ? fineTune - 16 : fineTune;
                s_FastTrackerPeriods[fineTune * 96 + note] = uint16_t(s_FastTrackerPeriod0[note] * pow(2, (-signedFineTune / 12.0) / 8.0) + 0.5);
            }
        }
    }
}

Song::PeriodDescriptor Song::m_TrackerPeriods[3] =
{
    { s_ProTrackerPeriods, 36, 12 },
    { s_TakeTrackerPeriods, 60, 0 },
    { s_FastTrackerPeriods, 96, 0 }
};

//https://weaselaudiolib.sourceforge.net/TakeTracker-FastTracker-notes-and-format.html

uint16_t Song::s_TakeTrackerPeriods[16 * 60] =
{
// Fine tuning 0, normal.
    1712, 1616, 1524, 1440, 1356, 1280, 1208, 1140, 1076, 1016,  960,  906,
     856,  808,  762,  720,  678,  640,  604,  570,  538,  508,  480,  453,
     428,  404,  381,  360,  339,  320,  302,  285,  269,  254,  240,  226,
     214,  202,  190,  180,  170,  160,  151,  143,  135,  127,  120,  113,
     107,  101,   95,   90,   85,   80,   76,   72,   68,   64,   60,   56,

// Fine tuning 1
    1700, 1604, 1514, 1430, 1348, 1274, 1202, 1134, 1070, 1010,  954,  900,
     850,  802,  757,  715,  674,  637,  601,  567,  535,  505,  477,  450,
     425,  401,  379,  357,  337,  318,  300,  284,  268,  253,  239,  225,
     213,  201,  189,  179,  169,  159,  150,  142,  134,  126,  119,  113,
     106,  100,   94,   89,   84,   79,   75,   71,   67,   63,   59,   56,
    
// Fine tuning 2
    1688, 1592, 1504, 1418, 1340, 1264, 1194, 1126, 1064, 1004,  948,  894,
     844,  796,  752,  709,  670,  632,  597,  563,  532,  502,  474,  447,
     422,  398,  376,  355,  335,  316,  298,  282,  266,  251,  237,  224,
     211,  199,  188,  177,  167,  158,  149,  141,  133,  125,  118,  112,
     105,   99,   93,   88,   83,   78,   74,   70,   66,   62,   59,   56,

// Fine tuning 3
    1676, 1582, 1492, 1408, 1330, 1256, 1184, 1118, 1056,  996,  940,  888,
     838,  791,  746,  704,  665,  628,  592,  559,  528,  498,  470,  444,
     419,  395,  373,  352,  332,  314,  296,  280,  264,  249,  235,  222,
     209,  198,  187,  176,  166,  157,  148,  140,  132,  125,  118,  111,
     104,   99,   93,   88,   83,   78,   74,   70,   66,   62,   59,   56,

// Fine tuning 4
    1664, 1570, 1482, 1398, 1320, 1246, 1176, 1110, 1048,  990,  934,  882,
     832,  785,  741,  699,  660,  623,  588,  555,  524,  495,  467,  441,
     416,  392,  370,  350,  330,  312,  294,  278,  262,  247,  233,  220,
     208,  196,  185,  175,  165,  156,  147,  139,  131,  124,  117,  110,
     104,   98,   92,   87,   82,   77,   73,   69,   65,   62,   58,   56,

// Fine tuning 5
    1652, 1558, 1472, 1388, 1310, 1238, 1168, 1102, 1040,  982,  926,  874,
     826,  779,  736,  694,  655,  619,  584,  551,  520,  491,  463,  437,
     413,  390,  368,  347,  328,  309,  292,  276,  260,  245,  232,  219,
     206,  195,  184,  174,  164,  155,  146,  138,  130,  123,  116,  109,
     103,   97,   92,   87,   82,   77,   73,   69,   65,   61,   58,   56,

// Fine tuning 6
    1640, 1548, 1460, 1378, 1302, 1228, 1160, 1094, 1032,  974,  920,  868,
     820,  774,  730,  689,  651,  614,  580,  547,  516,  487,  460,  434,
     410,  387,  365,  345,  325,  307,  290,  274,  258,  244,  230,  217,
     205,  193,  183,  172,  163,  154,  145,  137,  129,  122,  115,  109,
     102,   96,   91,   86,   81,   77,   72,   68,   64,   61,   57,   56,

// Fine tuning 7
    1628, 1536, 1450, 1368, 1292, 1220, 1150, 1086, 1026,  968,  914,  862,
     814,  768,  725,  684,  646,  610,  575,  543,  513,  484,  457,  431,
     407,  384,  363,  342,  323,  305,  288,  272,  256,  242,  228,  216,
     204,  192,  181,  171,  161,  152,  144,  136,  128,  121,  114,  108,
     102,   96,   90,   85,   80,   76,   72,   68,   64,   60,   57,   56,

// Fine tuning -8
    1814, 1712, 1616, 1524, 1440, 1356, 1280, 1208, 1140, 1076, 1016,  960,
     907,  856,  808,  762,  720,  678,  640,  604,  570,  538,  508,  480,
     453,  428,  404,  381,  360,  339,  320,  302,  285,  269,  254,  240,
     226,  214,  202,  190,  180,  170,  160,  151,  143,  135,  127,  120,
     113,  107,  101,   95,   90,   85,   80,   75,   71,   67,   63,   60,

// Fine tuning -7
    1800, 1700, 1604, 1514, 1430, 1350, 1272, 1202, 1134, 1070, 1010,  954,
     900,  850,  802,  757,  715,  675,  636,  601,  567,  535,  505,  477,
     450,  425,  401,  379,  357,  337,  318,  300,  284,  268,  253,  238,
     225,  212,  200,  189,  179,  169,  159,  150,  142,  134,  126,  119,
     112,  106,  100,   94,   89,   84,   79,   75,   71,   67,   63,   59,

// Fine tuning -6
    1788, 1688, 1592, 1504, 1418, 1340, 1264, 1194, 1126, 1064, 1004,  948,
     894,  844,  796,  752,  709,  670,  632,  597,  563,  532,  502,  474,
     447,  422,  398,  376,  355,  335,  316,  298,  282,  266,  251,  237,
     223,  211,  199,  188,  177,  167,  158,  149,  141,  133,  125,  118,
     111,  105,   99,   94,   88,   83,   79,   74,   70,   66,   62,   59,

// Fine tuning -5
    1774, 1676, 1582, 1492, 1408, 1330, 1256, 1184, 1118, 1056,  996,  940,
     887,  838,  791,  746,  704,  665,  628,  592,  559,  528,  498,  470,
     444,  419,  395,  373,  352,  332,  314,  296,  280,  264,  249,  235,
     222,  209,  198,  187,  176,  166,  157,  148,  140,  132,  125,  118,
     111,  104,   99,   93,   88,   83,   78,   74,   70,   66,   62,   59,

// Fine tuning -4
    1762, 1664, 1570, 1482, 1398, 1320, 1246, 1176, 1110, 1048,  988,  934,
     881,  832,  785,  741,  699,  660,  623,  588,  555,  524,  494,  467,
     441,  416,  392,  370,  350,  330,  312,  294,  278,  262,  247,  233,
     220,  208,  196,  185,  175,  165,  156,  147,  139,  131,  123,  117,
     110,  104,   98,   92,   87,   82,   78,   73,   69,   65,   61,   58,

// Fine tuning -3
    1750, 1652, 1558, 1472, 1388, 1310, 1238, 1168, 1102, 1040,  982,  926,
     875,  826,  779,  736,  694,  655,  619,  584,  551,  520,  491,  463,
     437,  413,  390,  368,  347,  328,  309,  292,  276,  260,  245,  232,
     219,  206,  195,  184,  174,  164,  155,  146,  138,  130,  123,  116,
     109,  103,   97,   92,   86,   82,   77,   73,   69,   65,   61,   58,

// Fine tuning -2
// B-4 = 57? seems a bit odd, values for fine tune -3 & -1 are both 58.
    1736, 1640, 1548, 1460, 1378, 1302, 1228, 1160, 1094, 1032,  974,  920,
     868,  820,  774,  730,  689,  651,  614,  580,  547,  516,  487,  460,
     434,  410,  387,  365,  345,  325,  307,  290,  274,  258,  244,  230,
     217,  205,  193,  183,  172,  163,  154,  145,  137,  129,  122,  115,
     108,  102,   96,   91,   86,   81,   77,   72,   68,   64,   61,   57,

// Fine tuning -1
    1724, 1628, 1536, 1450, 1368, 1292, 1220, 1150, 1086, 1026,  968,  914,
     862,  814,  768,  725,  684,  646,  610,  575,  543,  513,  484,  457,
     431,  407,  384,  363,  342,  323,  305,  288,  272,  256,  242,  228,
     216,  203,  192,  181,  171,  161,  152,  144,  136,  128,  121,  114,
     108,  101,   96,   90,   85,   80,   76,   72,   68,   64,   60,   58
};

uint16_t Song::s_ProTrackerPeriods[16 * 36] =
{
//Tuning 0
//  C-x  C#x  D-x  D#x  E-x  F-x  F#x  G-x  G#x  A-x  A#x  B-x
    856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480, 453,    //x = 1
    428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240, 226,    //x = 2 
    214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120, 113,    //x = 3

//Tuning 1
    850, 802, 757, 715, 674, 637, 601, 567, 535, 505, 477, 450,
    425, 401, 379, 357, 337, 318, 300, 284, 268, 253, 239, 225,
    213, 201, 189, 179, 169, 159, 150, 142, 134, 126, 119, 113,
//Tuning 2
    844, 796, 752, 709, 670, 632, 597, 563, 532, 502, 474, 447,
    422, 398, 376, 355, 335, 316, 298, 282, 266, 251, 237, 224,
    211, 199, 188, 177, 167, 158, 149, 141, 133, 125, 118, 112,

//Tuning 3
    838, 791, 746, 704, 665, 628, 592, 559, 528, 498, 470, 444,
    419, 395, 373, 352, 332, 314, 296, 280, 264, 249, 235, 222,
    209, 198, 187, 176, 166, 157, 148, 140, 132, 125, 118, 111,

//Tuning 4
    832, 785, 741, 699, 660, 623, 588, 555, 524, 495, 467, 441,
    416, 392, 370, 350, 330, 312, 294, 278, 262, 247, 233, 220,
    208, 196, 185, 175, 165, 156, 147, 139, 131, 124, 117, 110,

//Tuning 5
    826, 779, 736, 694, 655, 619, 584, 551, 520, 491, 463, 437,
    413, 390, 368, 347, 328, 309, 292, 276, 260, 245, 232, 219,
    206, 195, 184, 174, 164, 155, 146, 138, 130, 123, 116, 109,

//Tuning 6
    820, 774, 730, 689, 651, 614, 580, 547, 516, 487, 460, 434,
    410, 387, 365, 345, 325, 307, 290, 274, 258, 244, 230, 217,
    205, 193, 183, 172, 163, 154, 145, 137, 129, 122, 115, 109,

//Tuning 7
    814, 768, 725, 684, 646, 610, 575, 543, 513, 484, 457, 431,
    407, 384, 363, 342, 323, 305, 288, 272, 256, 242, 228, 216,
    204, 192, 181, 171, 161, 152, 144, 136, 128, 121, 114, 108,

//Tuning -8
    907, 856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480,
    453, 428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240,
    226, 214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120,

//Tuning -7
    900, 850, 802, 757, 715, 675, 636, 601, 567, 535, 505, 477,
    450, 425, 401, 379, 357, 337, 318, 300, 284, 268, 253, 238,
    225, 212, 200, 189, 179, 169, 159, 150, 142, 134, 126, 119,

//Tuning -6
    894, 844, 796, 752, 709, 670, 632, 597, 563, 532, 502, 474,
    447, 422, 398, 376, 355, 335, 316, 298, 282, 266, 251, 237,
    223, 211, 199, 188, 177, 167, 158, 149, 141, 133, 125, 118,

//Tuning -5
    887, 838, 791, 746, 704, 665, 628, 592, 559, 528, 498, 470,
    444, 419, 395, 373, 352, 332, 314, 296, 280, 264, 249, 235,
    222, 209, 198, 187, 176, 166, 157, 148, 140, 132, 125, 118,

//Tuning -4
    881, 832, 785, 741, 699, 660, 623, 588, 555, 524, 494, 467,
    441, 416, 392, 370, 350, 330, 312, 294, 278, 262, 247, 233,
    220, 208, 196, 185, 175, 165, 156, 147, 139, 131, 123, 117,

//Tuning -3
    875, 826, 779, 736, 694, 655, 619, 584, 551, 520, 491, 463,
    437, 413, 390, 368, 347, 328, 309, 292, 276, 260, 245, 232,
    219, 206, 195, 184, 174, 164, 155, 146, 138, 130, 123, 116,

//Tuning -2
    868, 820, 774, 730, 689, 651, 614, 580, 547, 516, 487, 460,
    434, 410, 387, 365, 345, 325, 307, 290, 274, 258, 244, 230,
    217, 205, 193, 183, 172, 163, 154, 145, 137, 129, 122, 115,

//Tuning -1
    862, 814, 768, 725, 684, 646, 610, 575, 543, 513, 484, 457,
    431, 407, 384, 363, 342, 323, 305, 288, 272, 256, 242, 228,
    216, 203, 192, 181, 171, 161, 152, 144, 136, 128, 121, 114,
};

uint16_t Song::s_FastTrackerPeriods[16 * 96] = { 0 };

const char* Song::s_NoteNames[96]
{
    "C-0", "C#0", "D-0", "D#0", "E-0", "F-0", "F#0", "G-0", "G#0", "A-0", "A#0", "B-0",
    "C-1", "C#1", "D-1", "D#1", "E-1", "F-1", "F#1", "G-1", "G#1", "A-1", "A#1", "B-1",
    "C-2", "C#2", "D-2", "D#2", "E-2", "F-2", "F#2", "G-2", "G#2", "A-2", "A#2", "B-2",
    "C-3", "C#3", "D-3", "D#3", "E-3", "F-3", "F#3", "G-3", "G#3", "A-3", "A#3", "B-3",
    "C-4", "C#4", "D-4", "D#4", "E-4", "F-4", "F#4", "G-4", "G#4", "A-4", "A#4", "B-4",
    "C-5", "C#5", "D-5", "D#5", "E-5", "F-5", "F#5", "G-5", "G#5", "A-5", "A#5", "B-5",
    "C-6", "C#6", "D-6", "D#6", "E-6", "F-6", "F#6", "G-6", "G#6", "A-6", "A#6", "B-6",
    "C-7", "C#7", "D-7", "D#7", "E-7", "F-7", "F#7", "G-7", "G#7", "A-7", "A#7", "B-7",
};

}