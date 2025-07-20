//Copyright 2025-Present riplin

#include <new>
#include <math.h>
#include <string.h>
#include <has/testing/log.h>
#include <ham/file/mod/mod.h>
#include <support/filestrm.h>

namespace Ham::File
{

Mod::Mod(Has::IAllocator& allocator)
    : m_Allocator(allocator)
    , m_Tracker(Tracker::ProTracker)
{
    memset(m_Name, 0, sizeof(m_Name));
    memset(m_Patterns, 0, sizeof(m_Patterns));
    CalculateFastTrackerNotePeriods();
}

Mod::~Mod()
{
    for (uint16_t i = 0; i < (sizeof(m_Patterns) / sizeof(Note*)); ++i)
    {
        if (m_Patterns[i] != nullptr)
            m_Allocator.Free(m_Patterns[i]);
    }

    for (uint16_t i = 0; i < (sizeof(m_Samples) / sizeof(Sample)); ++i)
    {
        if (m_Samples[i].Data != nullptr)
            m_Allocator.Free(m_Samples[i].Data);
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

void Mod::CalculateFastTrackerNotePeriods()
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
        if (mod->m_Samples[i].LoopEnd == 2)
            mod->m_Samples[i].LoopEnd = 0;

        mod->m_Samples[i].Data = nullptr;
        mod->m_Samples[i].Address = 0;
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
        }
    }

    mod->FindNotes();

    for (uint8_t i = 0; i < (sizeof(mod->m_Samples) / sizeof(Sample)); ++i)
    {
        if (mod->m_Samples[i].Length != 0)
        {
            mod->m_Samples[i].Data = allocator.AllocateAs<uint8_t>(mod->m_Samples[i].Length);
            if (!stream.Read(mod->m_Samples[i].Length, mod->m_Samples[i].Data))
            {
                Mod::Destroy(mod);
                return nullptr;
            }
        }
    }

    stream.Close();
    return mod;
}

void Mod::FindNotes()
{
    Tracker bestTracker = Tracker::ProTracker;
    uint32_t errors = 0xffffffff;
    for (uint8_t tracker = Tracker::ProTracker; tracker <= Tracker::FastTracker; ++tracker)
    {
        uint32_t e = SetNotes(Tracker(tracker));
        LOG("Mod", "Tracker: %i, errors: %i", tracker, e);
        if (e < errors)
        {
            bestTracker = Tracker(tracker);
            errors = e;
        }
    }
    SetNotes(bestTracker);
    m_Tracker = bestTracker;
}

uint32_t Mod::SetNotes(Tracker tracker)
{
    uint32_t errors = 0;
    for (uint8_t pattern = 0; pattern < m_PatternCount; ++pattern)
    {
        for (uint16_t rowNote = 0; rowNote < m_ChannelCount * 64; ++rowNote)
        {
            if (m_Patterns[pattern][rowNote].Period != 0)
            {
                for (uint8_t note = 0; note < m_TrackerPeriods[tracker].NoteCount; ++note)
                {
                    if ((m_Patterns[pattern][rowNote].Period > (m_TrackerPeriods[tracker].Periods[note] - 2)) &&
                        (m_Patterns[pattern][rowNote].Period < (m_TrackerPeriods[tracker].Periods[note] + 2)))
                    {
                        m_Patterns[pattern][rowNote].Note = note;
                        break;
                    }
                }
                if (m_Patterns[pattern][rowNote].Note == 0xFFFF)
                    ++errors;
            }
        }
    }
    return errors;
}


Mod::PeriodDescriptor Mod::m_TrackerPeriods[3] =
{
    { s_ProTrackerPeriods, 36, 12 },
    { s_TakeTrackerPeriods, 60, 0 },
    { s_FastTrackerPeriods, 96, 0 }
};

//https://weaselaudiolib.sourceforge.net/TakeTracker-FastTracker-notes-and-format.html

uint16_t Mod::s_TakeTrackerPeriods[16 * 60] =
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

uint16_t Mod::s_ProTrackerPeriods[16 * 36] =
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

uint16_t Mod::s_FastTrackerPeriods[16 * 96];

const char* Mod::s_NoteNames[96]
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