//Copyright 2025-Present riplin

#include <string.h>
#include <ham/music/song.h>
#include <has/testing/log.h>
#include <support/filestrm.h>

namespace Ham::File::Mod
{

static bool DetermineSongProperties(Has::IStream& stream, uint8_t& channelCount, uint8_t& sampleCount)
{
    auto startPosition = stream.Position();

    // Check magic at offset 1080
    if (!stream.SeekFromStart(0x438))
        return false;

    char magic[5] = { 0 };
    if (!stream.Read(4, magic))
        return false;

    if ((strcmp("M.K.", magic) == 0) ||
        (strcmp("FLT4", magic) == 0))
    {
        channelCount = 4;
        sampleCount = 31;
    }
    else if (strcmp("6CHN", magic) == 0)
    {
        channelCount = 6;
        sampleCount = 31;
    }
    else if ((strcmp("8CHN", magic) == 0) ||
             (strcmp("FLT8", magic) == 0))
    {
        channelCount = 8;
        sampleCount = 31;
    } // Some rare mods have this encoding:
    else if ((magic[2] == 'C') && (magic[3] == 'H') &&
            ((magic[0] >= '0') && magic[0] <= '3') &&
            ((magic[1] >= '0') && magic[0] <= '9'))
    {
        channelCount = ((magic[0] - '0') * 10) + (magic[1] - '0');
        sampleCount = 31;
    }
    else
    {
        // Assume a defautlt of 4.
        channelCount = 4;
        sampleCount = 15;
    }

    // Seek back to where we were.
    LOG("Mod", "Setting file position back to %i", startPosition);
    stream.SeekFromStart(startPosition);
    return true;
}

static bool ReadWord(Support::FileStream& stream, uint16_t& value)
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

Music::Song* Load(Has::IAllocator& allocator, const char* filePath)
{
    using namespace Has;
    using namespace Support;

    FileStream stream(filePath, FileStreamMode::Read);

    if (!stream.IsOpen())
    {
        LOG("Mod", "Failed to open file: [%s]", filePath);
        return nullptr;
    }

    Music::Song* song = Music::Song::Create(allocator);
    if (song == nullptr)
    {
        LOG("Mod", "Failure to create song");
        return nullptr;
    }

    uint8_t channelCount;
    uint8_t sampleCount;
    if (!DetermineSongProperties(stream, channelCount, sampleCount))
    {
        LOG("Mod", "Failure to determine file properties");
        Music::Song::Destroy(song);
        return nullptr;
    }
    
    song->SetChannelCount(channelCount);

    for (uint8_t channel = 0; channel < channelCount; ++channel)
    {
        song->SetChannelBalance(channel, ((channel & 1) == 0) ? 0x03 : 0x0C);
    }

    song->SetInstrumentCount(sampleCount);
    song->SetSpeed(6);
    song->SetBpm(125);
    song->SetVolume(64);
    song->SetOrderCount(128);

    char name[20];
    if (!stream.Read(sizeof(name), name))
    {
        LOG("Mod", "Error reading name from file");
        Music::Song::Destroy(song);
        return nullptr;
    }
    song->SetName(name, sizeof(name));

    for (uint16_t sample = 0; sample < sampleCount; ++sample)
    {
        song->SetSampleCount(sample, 1);

        char sampleName[22];
        if (!stream.Read(sizeof(sampleName), sampleName))
        {
            LOG("Mod", "Error reading sample %i name from file", sample);
            Music::Song::Destroy(song);
            return nullptr;
        }
        song->SetInstrumentName(sample, sampleName, sizeof(sampleName));
        song->SetSampleName(sample, 0, sampleName, sizeof(sampleName));

        uint16_t sampleLength = 0;
        if (!ReadWord(stream, sampleLength))
        {
            LOG("Mod", "Error reading sample %i length from file", sample);
            Music::Song::Destroy(song);
            return nullptr;
        }

        int8_t fineTune = 0;
        if (!stream.Read(sizeof(fineTune), &fineTune))
        {
            LOG("Mod", "Error reading sample %i fine tune from file", sample);
            Music::Song::Destroy(song);
            return nullptr;
        }
        
        uint8_t volume = 0;
        if (!stream.Read(sizeof(volume), &volume))
        {
            LOG("Mod", "Error reading sample %i volume from file", sample);
            Music::Song::Destroy(song);
            return nullptr;
        }

        uint16_t loopStart = 0;
        if (!ReadWord(stream, loopStart))
        {
            LOG("Mod", "Error reading sample %i loop start from file", sample);
            Music::Song::Destroy(song);
            return nullptr;
        }

        uint16_t loopEnd = 0;
        if (!ReadWord(stream, loopEnd))
        {
            LOG("Mod", "Error reading sample %i loop end from file", sample);
            Music::Song::Destroy(song);
            return nullptr;
        }

        if (loopEnd == 2)
            loopEnd = 0;

        loopEnd += loopStart;
            
        if (loopEnd > sampleLength)
            loopEnd = sampleLength;

        song->SetSampleLength(sample, 0, sampleLength, Music::Song::SampleWidth::BitWidth8);
        song->SetInstrumentMiddleC(sample, Music::Song::ConvertFineTuneToPeriod(fineTune));
        song->SetSampleVolume(sample, 0, volume);
        song->SetSampleLoopStart(sample, 0, loopStart);
        song->SetSampleLoopEnd(sample, 0, loopEnd);
    }

    uint8_t orderCount = 0;
    if (!stream.Read(sizeof(orderCount), &orderCount))
    {
        LOG("Mod", "Error reading order count from file");
        Music::Song::Destroy(song);
        return nullptr;
    }

    //Skip over restart value as it's not properly used by all trackers.
    stream.SeekFromCurrent(1);

    if (!song->LoadOrders<uint8_t>(128, stream))
    {
        LOG("Mod", "Error reading order list from file");
        Music::Song::Destroy(song);
        return nullptr;
    }

    //Now we have to determine the number of patterns by maxing the orders.
    uint8_t patternCount = 0;
    for (uint8_t i = 0; i < song->GetOrderCount(); ++i)
    {
        patternCount = max<uint8_t>(patternCount, song->GetOrder(i));
    }
    ++patternCount;

    song->SetOrderCount(orderCount);
    song->SetPatternCount(patternCount);

    //Skip over magic
    if (sampleCount > 15)
    {
        stream.SeekFromCurrent(4);
    }

    //Allocate and load the patterns
    for (uint8_t pattern = 0; pattern < patternCount; ++pattern)
    {
        song->SetPatternRowCount(pattern, 64);

        Music::Song::NoteData note;
        uint8_t data[4];
        for (uint16_t row = 0; row < song->GetPatternRowCount(pattern); ++row)
        {
            for (uint8_t channel = 0; channel < song->GetChannelCount(); ++channel)
            {
                if (!stream.Read(sizeof(data), data))
                {
                    LOG("Mod", "Error pattern note %i from row %i from pattern %i", channel, row, pattern);
                    Music::Song::Destroy(song);
                    return nullptr;
                }

                note.Instrument = (data[0] & 0xf0) | (data[2] >> 4);
                uint16_t period = uint16_t(data[1]) | (uint16_t(data[0] & 0x0f) << 8);
                note.Effect = data[2] & 0x0f;
                note.Parameter = data[3];
                if (period != 0)
                    note.Note = Music::Song::PeriodToNote(period) - 24;
                else
                    note.Note = Music::Note::NotSet;
                note.Volume = Music::Volume::NotSet;

                song->SetNote(pattern, row, channel, note);
            }
        }
    }

    for (uint8_t instrument = 0; instrument < song->GetInstrumentCount(); ++instrument)
    {
        const Music::Song::Sample* sample = song->GetSample(instrument, 0);
        if (sample->Length != 0)
        {
            uint32_t position = stream.Position() + sample->Length;

            if (sample->LoopEnd != 0)
                song->SetSampleLength(instrument, 0, min<uint16_t>(sample->Length, sample->LoopEnd), Music::Song::SampleWidth::BitWidth8);

            if (!song->LoadSampleData(instrument, 0, stream))
            {
                Music::Song::Destroy(song);
                return nullptr;
            }
            stream.SeekFromStart(position);
        }
    }

    stream.Close();

    LOG("Mod", "Song [%s] successfully loaded", filePath);
    return song;
}

}