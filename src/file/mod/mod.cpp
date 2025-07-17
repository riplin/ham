//Copyright 2025-Present riplin

#include <new>
#include <string.h>
#include <ham/file/mod/mod.h>
#include <support/filestrm.h>

namespace Ham::File
{

Mod::Mod(Has::IAllocator& allocator)
    : m_Allocator(allocator)
{
    memset(m_Name, 0, sizeof(m_Name));
    memset(m_Patterns, 0, sizeof(m_Patterns));
    memset(m_SampleData, 0, sizeof(m_SampleData));
}

Mod::~Mod()
{
    for (uint16_t i = 0; i < (sizeof(m_Patterns) / sizeof(Note*)); ++i)
    {
        if (m_Patterns[i] != nullptr)
            m_Allocator.Free(m_Patterns[i]);
    }

    for (uint16_t i = 0; i < (sizeof(m_SampleData) / sizeof(uint8_t*)); ++i)
    {
        if (m_SampleData[i] != nullptr)
            m_Allocator.Free(m_SampleData[i]);
    }
}

Mod* Mod::Create(Has::IAllocator& allocator)
{
    void* ptr = allocator.Allocate(sizeof(Mod));
    return ::new(ptr) Mod(allocator);
}

void Mod::Destroy(Mod* mod)
{
    Has::IAllocator& allocator = mod->m_Allocator;
    mod->~Mod();
    allocator.Free(mod);
}

static uint8_t LoadChannelCount(Support::FileStream& stream)
{
    // Save position
    uint32_t position = stream.Position();

    // Check magic at offset 1080
    if (!stream.SeekFromStart(0x438))
        return 0;

    char magic[5] = { 0 };
    if (!stream.Read(4, magic))
        return 0;

    uint8_t channels = 0;
    if (strcmp("M.K.", magic) == 0)
    {
        channels = 4;
    }

    if (strcmp("6CHN", magic) == 0)
    {
        channels = 6;
    }

    if (strcmp("8CHN", magic) == 0)
    {
        channels = 8;
    }

    // Some rare mods have this encoding:
    if ((magic[2] == 'C') && (magic[3] == 'H') &&
        ((magic[0] >= '1') && magic[0] <= '3') &&
        ((magic[1] >= '0') && magic[0] <= '9'))
    {
        uint8_t tmp = ((magic[0] - '0') * 10) + (magic[1] - '0');
        if ((tmp >= 10) && (tmp <= 32))
            channels = tmp;
    }

    // Back to where we were.
    stream.SeekFromStart(position);

    return channels;
}

bool ReadWord(Support::FileStream& stream, uint16_t& value)
{
    //Values are big-endian.
    uint8_t value1 = 0;
    if (!stream.Read(sizeof(value1), &value1))
        return false;

    uint8_t value0 = 0;
    if (!stream.Read(sizeof(value0), &value0))
        return false;

    // Swap the bytes and multiply by 2.
    value = (uint16_t(value1) << 9) | (uint16_t(value0) << 1);
    return true;
}

Mod* Mod::Load(Has::IAllocator& allocator, const char* fileName)
{
    using namespace Has;
    using namespace Support;
    FileStream stream(fileName, FileStreamMode::Read);

    if (!stream.IsOpen())
        return nullptr;

    uint8_t channels = LoadChannelCount(stream);

    if (channels == 0)
        return nullptr;

    Mod* mod = Mod::Create(allocator);

    mod->m_ChannelCount = channels;

    if (!stream.Read(sizeof(mod->m_Name) - 1, mod->m_Name))
    {
        Mod::Destroy(mod);
        return nullptr;
    }
    
    // Ensure the name is zero terminated.
    mod->m_Name[sizeof(mod->m_Name) - 1] = '\0';

    // Load sample information
    for (uint8_t i = 0; i < sizeof(mod->m_Samples) / sizeof(Mod::Sample); ++i)
    {
        //Read name
        if (!stream.Read(sizeof(mod->m_Samples[i].Name) - 1, mod->m_Samples[i].Name))
        {
            Mod::Destroy(mod);
            return nullptr;
        }

        // Ensure the name is zero terminated.
        mod->m_Samples[i].Name[sizeof(mod->m_Samples[i].Name) - 1] = '\0';

        if (!ReadWord(stream, mod->m_Samples[i].Length))
        {
            Mod::Destroy(mod);
            return nullptr;
        }

        int8_t fineTune = 0;
        if (!stream.Read(sizeof(fineTune), &fineTune))
        {
            Mod::Destroy(mod);
            return nullptr;
        }

        mod->m_Samples[i].FineTune = fineTune;
        
        if (!stream.Read(sizeof(mod->m_Samples[i].Volume), &mod->m_Samples[i].Volume))
        {
            Mod::Destroy(mod);
            return nullptr;
        }

        if (!ReadWord(stream, mod->m_Samples[i].LoopStart))
        {
            Mod::Destroy(mod);
            return nullptr;
        }

        if (!ReadWord(stream, mod->m_Samples[i].LoopEnd))
        {
            Mod::Destroy(mod);
            return nullptr;
        }
    }

    if (!stream.Read(sizeof(mod->m_OrderCount), &mod->m_OrderCount))
    {
        Mod::Destroy(mod);
        return nullptr;
    }

    //Skip over restart value as it's not properly used by all trackers.
    stream.SeekFromCurrent(1);

    if (!stream.Read(sizeof(mod->m_Orders), &mod->m_Orders))
    {
        Mod::Destroy(mod);
        return nullptr;
    }

    //Now we have to determine the number of patterns by maxing the orders.
    mod->m_PatternCount = 0;
    for (uint8_t i = 0; i < sizeof(mod->m_Orders); ++i)
    {
        mod->m_PatternCount = max<uint8_t>(mod->m_PatternCount, mod->m_Orders[i]);
    }
    ++mod->m_PatternCount;

    //Skip over magic
    stream.SeekFromCurrent(4);

    //Allocate and load the patterns
    for (uint8_t i = 0; i < mod->m_PatternCount; ++i)
    {
        mod->m_Patterns[i] = allocator.AllocateAs<Note>(sizeof(Note) * mod->m_ChannelCount * 64);//64 = rows

        uint8_t data[4];
        for (uint16_t j = 0; j < mod->m_ChannelCount * 64; ++j)
        {
            if (!stream.Read(4, data))
            {
                Mod::Destroy(mod);
                return nullptr;
            }

            mod->m_Patterns[i][j].Sample = (data[0] & 0xf0) | (data[2] >> 4);
            mod->m_Patterns[i][j].Period = uint16_t(data[1]) | (uint16_t(data[0] & 0x0f) << 8);
            mod->m_Patterns[i][j].Effect = data[2] & 0x0f;
            mod->m_Patterns[i][j].Parameter = data[3];

            mod->m_Patterns[i][j].Note = 0xFFFF;
            if (mod->m_Patterns[i][j].Period != 0)
            {
                for (uint8_t note = 0; note < 36; ++note)
                {
                    if ((mod->m_Patterns[i][j].Period > (s_Periods[0][note] - 2)) &&
                        (mod->m_Patterns[i][j].Period < (s_Periods[0][note] + 2)))
                    {
                        mod->m_Patterns[i][j].Note = note;
                        break;
                    }
                }
            }
        }
    }

    for (uint8_t i = 0; i < (sizeof(mod->m_Samples) / sizeof(Sample)); ++i)
    {
        if (mod->m_Samples[i].Length != 0)
        {
            mod->m_SampleData[i] = allocator.AllocateAs<uint8_t>(mod->m_Samples[i].Length);
            if (!stream.Read(mod->m_Samples[i].Length, mod->m_SampleData[i]))
            {
                Mod::Destroy(mod);
                return nullptr;
            }
        }
    }

    stream.Close();
    return mod;
}

uint16_t Mod::s_Periods[16][36] =
{
    {//Tuning 0
    //  C-x C#x D-x D#x E-x F-x F#x G-x G#x A-x A#x B-x
        856,808,762,720,678,640,604,570,538,508,480,453,    //x = 1
        428,404,381,360,339,320,302,285,269,254,240,226,    //x = 2 
        214,202,190,180,170,160,151,143,135,127,120,113     //x = 3
    },
    {//Tuning 1
        850,802,757,715,674,637,601,567,535,505,477,450,
        425,401,379,357,337,318,300,284,268,253,239,225,
        213,201,189,179,169,159,150,142,134,126,119,113
    },
    {//Tuning 2
        844,796,752,709,670,632,597,563,532,502,474,447,
        422,398,376,355,335,316,298,282,266,251,237,224,
        211,199,188,177,167,158,149,141,133,125,118,112
    },
    {//Tuning 3
        838,791,746,704,665,628,592,559,528,498,470,444,
        419,395,373,352,332,314,296,280,264,249,235,222,
        209,198,187,176,166,157,148,140,132,125,118,111
    },
    {//Tuning 4
        832,785,741,699,660,623,588,555,524,495,467,441,
        416,392,370,350,330,312,294,278,262,247,233,220,
        208,196,185,175,165,156,147,139,131,124,117,110
    },
    {//Tuning 5
        826,779,736,694,655,619,584,551,520,491,463,437,
        413,390,368,347,328,309,292,276,260,245,232,219,
        206,195,184,174,164,155,146,138,130,123,116,109
    },
    {//Tuning 6
        820,774,730,689,651,614,580,547,516,487,460,434,
        410,387,365,345,325,307,290,274,258,244,230,217,
        205,193,183,172,163,154,145,137,129,122,115,109
    },
    {//Tuning 7
        814,768,725,684,646,610,575,543,513,484,457,431,
        407,384,363,342,323,305,288,272,256,242,228,216,
        204,192,181,171,161,152,144,136,128,121,114,108
    },
    {//Tuning -8
        907,856,808,762,720,678,640,604,570,538,508,480,
        453,428,404,381,360,339,320,302,285,269,254,240,
        226,214,202,190,180,170,160,151,143,135,127,120
    },
    {//Tuning -7
        900,850,802,757,715,675,636,601,567,535,505,477,
        450,425,401,379,357,337,318,300,284,268,253,238,
        225,212,200,189,179,169,159,150,142,134,126,119
    },
    {//Tuning -6
        894,844,796,752,709,670,632,597,563,532,502,474,
        447,422,398,376,355,335,316,298,282,266,251,237,
        223,211,199,188,177,167,158,149,141,133,125,118
    },
    {//Tuning -5
        887,838,791,746,704,665,628,592,559,528,498,470,
        444,419,395,373,352,332,314,296,280,264,249,235,
        222,209,198,187,176,166,157,148,140,132,125,118
    },
    {//Tuning -4
        881,832,785,741,699,660,623,588,555,524,494,467,
        441,416,392,370,350,330,312,294,278,262,247,233,
        220,208,196,185,175,165,156,147,139,131,123,117
    },
    {//Tuning -3
        875,826,779,736,694,655,619,584,551,520,491,463,
        437,413,390,368,347,328,309,292,276,260,245,232,
        219,206,195,184,174,164,155,146,138,130,123,116
    },
    {//Tuning -2
        868,820,774,730,689,651,614,580,547,516,487,460,
        434,410,387,365,345,325,307,290,274,258,244,230,
        217,205,193,183,172,163,154,145,137,129,122,115
    },
    {//Tuning -1
        862,814,768,725,684,646,610,575,543,513,484,457,
        431,407,384,363,342,323,305,288,272,256,242,228,
        216,203,192,181,171,161,152,144,136,128,121,114
    }
};

const char* Mod::s_Notes[36]
{
    "C-1", "C#1", "D-1", "D#1", "E-1", "F-1", "F#1", "G-1", "G#1", "A-1", "A#1", "B-1",
    "C-2", "C#2", "D-2", "D#2", "E-2", "F-2", "F#2", "G-2", "G#2", "A-2", "A#2", "B-2",
    "C-3", "C#3", "D-3", "D#3", "E-3", "F-3", "F#3", "G-3", "G#3", "A-3", "A#3", "B-3"
};

}