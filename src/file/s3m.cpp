//Copyright 2025-Present riplin

#include <string.h>
#include <ham/music/song.h>
#include <has/testing/log.h>
#include <support/filestrm.h>

namespace Ham::File::S3m
{

typedef uint16_t Flags_t;
namespace Flags
{
    enum
    {
        ScreamTracker2Vibrato = 0x01,
        ScreamTracker2Tempo = 0x02,
        AmigaSlides = 0x04,
        ZeroVolumeOptimizations = 0x08,
        AmigaLimits = 0x10,
        EnableSoundBlasterFilter = 0x20,
        ScreamTracker3VolumeSlides = 0x40,
        SpecialPointerIsValid = 0x80
    };
}

typedef uint16_t TrackerVersion_t;
namespace TrackerVersion
{
    enum
    {
        ScreamTracker = 0x1000,
        ImagoOrpheus = 0x2000,
        ImpulseTracker = 0x3000,
        SchismTracker = 0x4000,
        OpenMPT = 0x5000,
        BeRoTracker = 0x6000,
        CreamTracker = 0x7000,
        Camoto = 0xC000,//0xCA00
        Mask = 0xF000,
        MajorVersion = 0x0F00,
        MinorVersion = 0x00FF,
    };
}

typedef uint8_t InstrumentFlags_t;
namespace InstrumentFlags
{
    enum
    {
        Looping = 0x01,
        Stereo = 0x02,
        Bits16 = 0x04
    };
}

typedef uint8_t NoteFlags_t;
namespace NoteFlags
{
    enum
    {
        Channel = 0x1F,
        NoteAndInstrumentPresent = 0x20,
        VolumePresent = 0x40,
        EffectAndParameterPresent = 0x80
    };
}

typedef uint8_t Note_t;
namespace Note
{
    enum
    {
        Semitone = 0x0F,
        Octave = 0xF0,
        KeyOff = 0xFE,
        NotSet = 0xFF
    };

    namespace Shift
    {
        enum
        {
            Semitone = 0x00,
            Octave = 0x04
        };
    }
}

typedef uint8_t Effect_t;
namespace Effect
{
    enum
    {
        NotSet = 0x00,
        SetSpeed = 0x01,                    // Axx Set speed to xx
        JumpToPattern = 0x02,               // Bxx Pattern jump                 // MOD Bxx
        PatternBreak = 0x03,                // Cxx Pattern break                // MOD Dxx
        VolumeSlide = 0x04,                 // Dxy Volume slide
        PortamentoDown = 0x05,              // Exy Portamento down
        PortamentoUp = 0x06,                // Fxy Portamento up
        PortamentoToNote = 0x07,            // Gxx Portamento to note           // MOD 3xx
        Vibrato = 0x08,                     // Hxy Vibrato                      // MOD 4xy
        Tremor = 0x09,                      // Ixy Tremor
        Arpeggio = 0x0a,                    // Jxy Arpeggio                     // MOD 0xy
        VibratoAndVolumeSlide = 0x0b,       // Kxy Vibrato + volume slide
        PortamentoAndVolumeSlide = 0x0c,    // Lxy Portamento + volume slide
        SampleOffset = 0x0f,                // Oxx Sample offset                // MOD 9xx
        RetriggerAndVolumeSlide = 0x11,     // Qxy Retrigger + volume slide
        Tremolo = 0x12,                     // Rxy Tremolo                      // MOD 7xy
        Special = 0x13,                     // S effects. See next enum
        SetTempo = 0x14,                    // Txx Set tempo
        FineVibrato = 0x15,                 // Uxy Fine vibrato
        GlobalVolume = 0x16,                // Vxx Global volume
        Pan = 0x18,                         // Xxx Pan                          // MOD 8xx
    };
}

typedef uint8_t Special_t;
namespace Special
{
    enum
    {
        Mask = 0xF0,
        SetFilter = 0x00,                   // S0x Set filter on/off            // MOD E0x
        GlissandoControl = 0x10,            // S1x Set Glissando on/off         // MOD E3x
        SetFineTune = 0x20,                 // S2x Set Fine tune                // MOD E5x
        SetVibratoWaveform = 0x30,          // S3x Set vibrato waveform         // MOD E4x
        SetTremoloWaveform = 0x40,          // S4x Set tremolo waveform         // MOD E7x
        FinePanning = 0x80,                 // S8x Set pan position             // MOD E8x
        StereoControl = 0xA0,               // SAx Stereo control
        PatternLoop = 0xB0,                 // SBx Pattern loop                 // MOD E6x
        CutNote = 0xC0,                     // SCx Cut note                     // MOD ECx
        DelayNote = 0xD0,                   // SDx Delay note                   // MOD EDx
        PatternDelay = 0xE0,                // SEx Pattern delay                // MOD EEx
        FunkRepeat = 0xF0,                  // SFx Funk repeat (invert loop)    // MOD EFx
    };
};

Music::Song* Load(Has::IAllocator& allocator, const char* filePath)
{
    using namespace Has;
    using namespace Support;

    FileStream stream(filePath, FileStreamMode::Read);

    if (!stream.IsOpen())
    {
        LOG("S3m", "Failed to open file: [%s]", filePath);
        return nullptr;
    }

    // Skip to the magic marker location.
    if (!stream.SeekFromStart(0x2C))
    {
        LOG("S3m", "Error seeking to offset 0x2C");
        return nullptr;
    }

    char sig2[5] = { 0 };
    if (!stream.Read(sizeof(sig2) - 1, &sig2))
    {
        LOG("S3m", "Error reading signature 2 from file");
        return nullptr;
    }

    if (strcmp(sig2, "SCRM") != 0)
    {
        LOG("S3m", "Signature 2 does not match");
        return nullptr;
    }

    // Back to the beginning of the file.
    if (!stream.SeekFromStart(0x00))
    {
        LOG("S3m", "Error seeking to offset 0x00");
        return nullptr;
    }

    Music::Song* song = Music::Song::Create(allocator);
    if (song == nullptr)
    {
        LOG("S3m", "Failure to create song");
        return nullptr;
    }

    char name[28];
    if (!stream.Read(sizeof(name), name))
    {
        LOG("S3m", "Error reading name from file");
        Music::Song::Destroy(song);
        return nullptr;
    }
    song->SetName(name, sizeof(name));

    // Skip over magic numbers that aren't stable for testing.
    if (!stream.SeekFromStart(0x20))
    {
        LOG("S3m", "Error seeking to offset 0x20");
        Music::Song::Destroy(song);
        return nullptr;
    }

    uint16_t orderCount = 0;
    if (!stream.Read(sizeof(orderCount), &orderCount))
    {
        LOG("S3m", "Error reading order count from file");
        Music::Song::Destroy(song);
        return nullptr;
    }

    uint16_t instrumentCount = 0;
    if (!stream.Read(sizeof(instrumentCount), &instrumentCount))
    {
        LOG("S3m", "Error reading instrument count from file");
        Music::Song::Destroy(song);
        return nullptr;
    }
    song->SetInstrumentCount(instrumentCount);

    uint16_t patternCount = 0;
    if (!stream.Read(sizeof(patternCount), &patternCount))
    {
        LOG("S3m", "Error reading pattern count from file");
        Music::Song::Destroy(song);
        return nullptr;
    }

    Flags_t flags = 0;
    if (!stream.Read(sizeof(flags), &flags))
    {
        LOG("S3m", "Error reading flags from file");
        Music::Song::Destroy(song);
        return nullptr;
    }

    uint16_t trackerVersion = 0;
    if (!stream.Read(sizeof(trackerVersion), &trackerVersion))
    {
        LOG("S3m", "Error reading tracker version from file");
        Music::Song::Destroy(song);
        return nullptr;
    }

    // Seek to second section.
    if (!stream.SeekFromStart(0x30))
    {
        LOG("S3m", "Error seeking to offset 0x30");
        Music::Song::Destroy(song);
        return nullptr;
    }
    
    uint8_t globalVolume = 0;
    if (!stream.Read(sizeof(globalVolume), &globalVolume))
    {
        LOG("S3m", "Error reading global volume from file");
        Music::Song::Destroy(song);
        return nullptr;
    }
    song->SetVolume(globalVolume);

    uint8_t initialSpeed = 0;
    if (!stream.Read(sizeof(initialSpeed), &initialSpeed))
    {
        LOG("S3m", "Error reading initial speed from file");
        Music::Song::Destroy(song);
        return nullptr;
    }
    song->SetSpeed(initialSpeed);

    uint8_t initialBpm = 0;
    if (!stream.Read(sizeof(initialBpm), &initialBpm))
    {
        LOG("S3m", "Error reading initial tempo from file");
        Music::Song::Destroy(song);
        return nullptr;
    }
    song->SetBpm(initialBpm);

    uint8_t masterVolume = 0;
    if (!stream.Read(sizeof(masterVolume), &masterVolume))
    {
        LOG("S3m", "Error reading master volume from file");
        Music::Song::Destroy(song);
        return nullptr;
    }
    LOG("S3m", "Master volume is 0x%02X", masterVolume);

    // Skip over ultra click removal
    if (!stream.SeekFromCurrent(1))
    {
        LOG("S3m", "Error seeking past 1 byte");
        Music::Song::Destroy(song);
        return nullptr;
    }

    uint8_t defaultPan = 0;
    if (!stream.Read(sizeof(defaultPan), &defaultPan))
    {
        LOG("S3m", "Error reading ultra default pan from file");
        Music::Song::Destroy(song);
        return nullptr;
    }
    LOG("S3m", "Default pan is 0x%02X", defaultPan);

    // Seek to third section.
    if (!stream.SeekFromStart(0x40))
    {
        LOG("S3m", "Error seeking to offset 0x40");
        Music::Song::Destroy(song);
        return nullptr;
    }

    uint8_t channels[32] = { 0 };
    if (!stream.Read(sizeof(channels), &channels))
    {
        LOG("S3m", "Error reading channels from file");
        Music::Song::Destroy(song);
        return nullptr;
    }

    uint8_t channelCount = 0;
    uint8_t channelRemap[32] = { 0xFF };
    uint8_t channelPan[32] = { 0x00 };
    for (uint8_t channel = 0; channel < 32; ++channel)
    {
        if (channels[channel] < 16)
        {
            channelRemap[channel] = channelCount;
            channelPan[channelCount] = ((channel & 1) == 0) ? 0x03 : 0x0C;
            ++channelCount;
        }
    }
    song->SetChannelCount(channelCount);

    uint8_t* orders = allocator.AllocateAs<uint8_t>(orderCount);
    if (orders == nullptr)
    {
        LOG("S3m", "Could not allocate memory for %i orders", orders);
        Music::Song::Destroy(song);
        return nullptr;
    }

    if (!stream.Read(orderCount, orders))
    {
        LOG("S3m", "Error reading %i orders from file", orderCount);
        Music::Song::Destroy(song);
        return nullptr;
    }

    uint8_t realPatternCount = 0;
    uint16_t realOrderCount = 0;
    for (uint16_t order = 0; order < orderCount; ++order)
    {
        if (orders[order] == 0xFE)
            continue;

        if (orders[order] == 0xFF)
            break;

        song->SetOrder(realOrderCount, orders[order]);
        realPatternCount = max<uint16_t>(realPatternCount, orders[order]);

        ++realOrderCount;
    }
    allocator.Free(orders);
    orders = nullptr;
    song->SetOrderCount(realOrderCount);
    song->SetPatternCount(realPatternCount);

    for (uint16_t instrument = 0; instrument < instrumentCount; ++instrument)
    {
        uint16_t instrumentPtr = 0;
        if (!stream.Read(sizeof(instrumentPtr), &instrumentPtr))
        {
            LOG("S3m", "Error reading instrument pointer from file");
            Music::Song::Destroy(song);
            return nullptr;
        }
        uint32_t position = stream.Position();

        // Seek to instrument and skip filename stored at the start.
        if (!stream.SeekFromStart((uint32_t(instrumentPtr) << 4) + 13))
        {
            LOG("S3m", "Error seeking to instrument %i at offset 0x%05lX", instrument, (uint32_t(instrumentPtr) << 4) + 13);
            Music::Song::Destroy(song);
            return nullptr;
        }

        uint8_t sampleOffsetHi = 0;
        if (!stream.Read(sizeof(sampleOffsetHi), &sampleOffsetHi))
        {
            LOG("S3m", "Error reading sample offset high byte for instrument %i", instrument);
            Music::Song::Destroy(song);
            return nullptr;
        }

        uint16_t sampleOffsetLo = 0;
        if (!stream.Read(sizeof(sampleOffsetLo), &sampleOffsetLo))
        {
            LOG("S3m", "Error reading sample offset low word for instrument %i", instrument);
            Music::Song::Destroy(song);
            return nullptr;
        }

        uint32_t sampleOffset = ((uint32_t(sampleOffsetHi) << 16) | uint32_t(sampleOffsetLo)) << 4;

        uint32_t sampleLength = 0;
        if (!stream.Read(sizeof(sampleLength), &sampleLength))
        {
            LOG("S3m", "Error reading sample length for instrument %i", instrument);
            Music::Song::Destroy(song);
            return nullptr;
        }

        uint32_t sampleLoopStart = 0;
        if (!stream.Read(sizeof(sampleLoopStart), &sampleLoopStart))
        {
            LOG("S3m", "Error reading sample loop start for instrument %i", instrument);
            Music::Song::Destroy(song);
            return nullptr;
        }

        uint32_t sampleLoopEnd = 0;
        if (!stream.Read(sizeof(sampleLoopEnd), &sampleLoopEnd))
        {
            LOG("S3m", "Error reading sample loop end for instrument %i", instrument);
            Music::Song::Destroy(song);
            return nullptr;
        }

        uint8_t sampleVolume = 0;
        if (!stream.Read(sizeof(sampleVolume), &sampleVolume))
        {
            LOG("S3m", "Error reading sample volume for instrument %i", instrument);
            Music::Song::Destroy(song);
            return nullptr;
        }

        if (!stream.SeekFromCurrent(2))
        {
            LOG("S3m", "Error seeking past 2 bytes in instrument %i", instrument);
            Music::Song::Destroy(song);
            return nullptr;
        }

        InstrumentFlags_t instrumentFlags = 0;
        if (!stream.Read(sizeof(instrumentFlags), &instrumentFlags))
        {
            LOG("S3m", "Error reading instrument flags for instrument %i", instrument);
            Music::Song::Destroy(song);
            return nullptr;
        }
        LOG("S3m", "Instrument flags: 0x%02X", instrumentFlags);

        uint16_t middleC = 0;
        if (!stream.Read(sizeof(middleC), &middleC))
        {
            LOG("S3m", "Error reading middle C for instrument %i", instrument);
            Music::Song::Destroy(song);
            return nullptr;
        }
        if (middleC == 0)
            middleC = Music::Note::MiddleC;

        if (!stream.SeekFromCurrent(14))
        {
            LOG("S3m", "Error seeking past 14 bytes in instrument %i", instrument);
            Music::Song::Destroy(song);
            return nullptr;
        }

        char sampleName[28] = { 0 };
        if (!stream.Read(sizeof(sampleName), &sampleName))
        {
            LOG("S3m", "Error reading name for instrument %i", instrument);
            Music::Song::Destroy(song);
            return nullptr;
        }

        if ((instrumentFlags & InstrumentFlags::Looping) != 0)
        {
            if (sampleLoopEnd <= 2)
                sampleLoopEnd = 0;
            if (sampleLoopEnd > sampleLength)
                sampleLoopEnd = sampleLength;
            sampleLength = sampleLoopEnd;//Can set length to 0?
        }
        else
        {
            sampleLoopStart = sampleLoopEnd = 0;
        }

        song->SetSampleCount(instrument, 1);
        song->SetInstrumentMiddleC(instrument, middleC);
        song->SetSampleName(instrument, 0, sampleName, sizeof(sampleName));
        song->SetInstrumentName(instrument, sampleName, sizeof(sampleName));
        song->SetSampleLength(instrument, 0, sampleLength, ((instrumentFlags & InstrumentFlags::Bits16) != 0) ? Music::Song::SampleWidth::BitWidth16 : Music::Song::SampleWidth::BitWidth8);
        song->SetSampleLoopStart(instrument, 0, sampleLoopStart);
        song->SetSampleLoopEnd(instrument, 0, sampleLoopEnd);
        song->SetSampleVolume(instrument, 0, sampleVolume);

        if (!stream.SeekFromStart(sampleOffset))
        {
            LOG("S3m", "Error seeking back to instrument sample data at offset 0x%08lX", sampleOffset);
            Music::Song::Destroy(song);
            return nullptr;
        }

        song->LoadSampleData(instrument, 0, stream);

        if (!stream.SeekFromStart(position))
        {
            LOG("S3m", "Error seeking back to instrument pointer list at offset 0x%04lX", position);
            Music::Song::Destroy(song);
            return nullptr;
        }
    }

    for (uint16_t pattern = 0; pattern < patternCount; ++pattern)
    {
        uint16_t patternPtr = 0;
        if (!stream.Read(sizeof(patternPtr), &patternPtr))
        {
            LOG("S3m", "Error reading pattern pointer from file");
            Music::Song::Destroy(song);
            return nullptr;
        }
        uint32_t position = stream.Position();

        if (!stream.SeekFromStart((uint32_t(patternPtr) << 4) + 2))
        {
            LOG("S3m", "Error seeking to pattern %i at offset 0x%05lX", pattern, (uint32_t(patternPtr) << 4) + 2);
            Music::Song::Destroy(song);
            return nullptr;
        }

        Music::Song::NoteData note;
        note.Note = Music::Note::NotSet;
        note.Volume = Music::Volume::NotSet;
        song->SetPatternRowCount(pattern, 64);
        uint8_t row = 0;
        while (row < 64)
        {
            NoteFlags_t noteFlags = 0;
            if (!stream.Read(sizeof(noteFlags), &noteFlags))
            {
                LOG("S3m", "Error reading note flags for pattern %i row %i", pattern, row);
                Music::Song::Destroy(song);
                return nullptr;
            }

            if (noteFlags == 0)
            {
                ++row;
                continue;
            }

            uint8_t channel = channelRemap[noteFlags & NoteFlags::Channel];

            note.Instrument = 0;
            note.Note = Music::Note::NotSet;
            if ((noteFlags & NoteFlags::NoteAndInstrumentPresent))
            {
                uint8_t noteData = 0;
                if (!stream.Read(sizeof(noteData), &noteData))
                {
                    LOG("S3m", "Error reading note data for pattern %i row %i channel %i", pattern, row, channel);
                    Music::Song::Destroy(song);
                    return nullptr;
                }
                
                switch(noteData)
                {
                case Note::KeyOff:
                    note.Note = Music::Note::KeyOff;
                    break;
                case Note::NotSet:
                    note.Note = Music::Note::NotSet;
                    break;
                default:
                    note.Note = ((noteData & Note::Octave) >> Note::Shift::Octave) * 12 + (noteData & Note::Semitone);
                }

                uint8_t instrument = 0;
                if (!stream.Read(sizeof(instrument), &instrument))
                {
                    LOG("S3m", "Error reading instrument for pattern %i row %i channel %i", pattern, row, channel);
                    Music::Song::Destroy(song);
                    return nullptr;
                }
                note.Instrument = instrument + 1;
            }


            note.Volume = Music::Volume::NotSet;
            if ((noteFlags & NoteFlags::VolumePresent) != 0)
            {
                uint8_t noteVolume = 0;
                if (!stream.Read(sizeof(noteVolume), &noteVolume))
                {
                    LOG("S3m", "Error reading volume for pattern %i row %i channel %i", pattern, row, channel);
                    Music::Song::Destroy(song);
                    return nullptr;
                }
                if (noteVolume == 0xFF)
                    note.Volume = Music::Volume::NotSet;
                else if (noteVolume > Music::Volume::Max)
                    note.Volume = Music::Volume::Max;
                else
                    note.Volume = noteVolume;
            }

            note.Effect = 0;
            note.Parameter = 0;
            if ((noteFlags & NoteFlags::EffectAndParameterPresent) != 0)
            {
                uint8_t effect = 0;
                if (!stream.Read(sizeof(effect), &effect))
                {
                    LOG("S3m", "Error reading effect for pattern %i row %i channel %i", pattern, row, channel);
                    Music::Song::Destroy(song);
                    return nullptr;
                }

                uint8_t parameter = 0;
                if (!stream.Read(sizeof(parameter), &parameter))
                {
                    LOG("S3m", "Error reading parameter for pattern %i row %i channel %i", pattern, row, channel);
                    Music::Song::Destroy(song);
                    return nullptr;
                }

                note.Parameter = parameter;
                switch (effect)
                {
                case Effect::SetSpeed:
                    note.Effect = Music::Effect::SetSpeedEx;
                    break;
                case Effect::JumpToPattern:             // MOD Bxx
                    note.Effect = Music::Effect::JumpToPattern;
                    break;
                case Effect::PatternBreak:              // MOD Dxx
                    note.Effect = Music::Effect::PatternBreak;
                    break;
                case Effect::VolumeSlide:
                    note.Effect = Music::Effect::VolumeSlideEx;
                    break;
                case Effect::PortamentoDown:
                    note.Effect = Music::Effect::PortamentoDownEx;
                    break;
                case Effect::PortamentoUp:
                    note.Effect = Music::Effect::PortamentoUpEx;
                    break;
                case Effect::PortamentoToNote:          // MOD 3xx
                    note.Effect = Music::Effect::PortamentoToNote;
                    break;
                case Effect::Vibrato:                   // MOD 4xy
                    note.Effect = Music::Effect::Vibrato;
                    break;
                case Effect::Tremor:
                    note.Effect = Music::Effect::Tremor;
                    break;
                case Effect::Arpeggio:                  // MOD 0xy
                    note.Effect = Music::Effect::Arpeggio;
                    break;
                case Effect::VibratoAndVolumeSlide:
                    note.Effect = Music::Effect::VibratoAndVolumeSlideEx;
                    break;
                case Effect::PortamentoAndVolumeSlide:
                    note.Effect = Music::Effect::PortamentoAndVolumeSlideEx;
                    break;
                case Effect::SampleOffset:              // MOD 9xx
                    note.Effect = Music::Effect::SampleOffset;
                    break;
                case Effect::RetriggerAndVolumeSlide:
                    note.Effect = Music::Effect::RetriggerAndVolumeSlide;
                    break;
                case Effect::Tremolo:                   // MOD 7xy
                    note.Effect = Music::Effect::Tremolo;
                    break;
                case Effect::Special:                   // MOD Exy
                    note.Effect = Music::Effect::MoreEffects;
                    switch (parameter & Special::Mask)
                    {
                    case Special::SetFilter:            // MOD E0x
                        note.Parameter = Music::SubEffect::SetFilter | (parameter & ~Special::Mask);
                        break;
                    case Special::GlissandoControl:     // MOD E3x
                        note.Parameter = Music::SubEffect::GlissandoControl | (parameter & ~Special::Mask);
                        break;
                    case Special::SetFineTune:          // MOD E5x
                        note.Parameter = Music::SubEffect::SetFinetune | (parameter & ~Special::Mask);
                        break;
                    case Special::SetVibratoWaveform:   // MOD E4x
                        note.Parameter = Music::SubEffect::SetVibratoWaveform | (parameter & ~Special::Mask);
                        break;
                    case Special::SetTremoloWaveform:   // MOD E7x
                        note.Parameter = Music::SubEffect::SetTremoloWaveform | (parameter & ~Special::Mask);
                        break;
                    case Special::FinePanning:          // MOD E8x
                        note.Parameter = Music::SubEffect::FinePanning | (parameter & ~Special::Mask);
                        break;
                    case Special::StereoControl:
                        note.Effect = Music::Effect::StereoControl;
                        note.Parameter = parameter & ~Special::Mask;
                        break;
                    case Special::PatternLoop:          // MOD E6x
                        note.Parameter = Music::SubEffect::PatternLoop | (parameter & ~Special::Mask);
                        break;
                    case Special::CutNote:              // MOD ECx
                        note.Parameter = Music::SubEffect::CutNote | (parameter & ~Special::Mask);
                        break;
                    case Special::DelayNote:            // MOD EDx
                        note.Parameter = Music::SubEffect::DelayNote | (parameter & ~Special::Mask);
                        break;
                    case Special::PatternDelay:         // MOD EEx
                        note.Parameter = Music::SubEffect::PatternDelay | (parameter & ~Special::Mask);
                        break;
                    case Special::FunkRepeat:           // MOD EFx
                        note.Parameter = Music::SubEffect::InvertLoop | (parameter & ~Special::Mask);
                        break;
                    }
                    break;
                case Effect::SetTempo:
                    note.Effect = Music::Effect::SetTempo;
                    break;
                case Effect::FineVibrato:
                    note.Effect = Music::Effect::FineVibrato;
                    break;
                case Effect::GlobalVolume:
                    note.Effect = Music::Effect::GlobalVolume;
                    break;
                case Effect::Pan:                       // MOD 8xx
                    note.Effect = Music::Effect::Pan;
                    break;
                default:
                    note.Effect = 0;
                    note.Parameter = 0;
                    break;
                }

            }

            if (channel < channelCount)
            {
                song->SetNote(pattern, row, channel, note);
            }
        }

        if (!stream.SeekFromStart(position))
        {
            LOG("S3m", "Error seeking back to pattern pointer list at offset 0x%04lX", position);
            Music::Song::Destroy(song);
            return nullptr;
        }
    }

    if (defaultPan == 0xFC)
    {
        uint8_t panData[32] = { 0 };
        if (!stream.Read(sizeof(panData), &panData))
        {
            LOG("S3m", "Error reading channel balance data from file");
            Music::Song::Destroy(song);
            return nullptr;
        }
        for (uint8_t channel = 0; channel < sizeof(panData); channel++)
        {
            if ((panData[channel] & 0x10) != 0)
            {
                channelPan[channelRemap[channel]] = panData[channel] & 0x0F;
            }
        }
    }

    for (uint8_t channel = 0; channel < channelCount; ++channel)
    {
        song->SetChannelBalance(channel, ((masterVolume & 0x80) != 0) ? channelPan[channel] : 0x07);
    }

    //Song::Destroy(song);
    stream.Close();
    return song;
}

}