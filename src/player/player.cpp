//Copyright 2025-Present riplin

#include <ham/player/player.h>
#include <ham/drivers/gravis/shared/system.h>

namespace Ham::Player
{

void Player::CalculateNextValues()
{
    if ((m_CurrentTick + 1) >= m_Speed)
    {
        m_NextTick = 0;
        if (m_CurrentRow + 1 >= 64)
        {
            m_NextRow = 0;
            m_NextOrderIndex = m_CurrentOrderIndex + 1;
        }
        else
        {
            m_NextRow = m_CurrentRow + 1;
        }
    }
    else
    {
        m_NextTick = m_CurrentTick + 1;
    }
}

void Player::Tick()
{
    if (m_State != State::Playing)
        return;
    
    if (m_PatternDelay == 0)
    {
        if (m_CurrentTick == 0) // next row.
        {
            for (uint8_t channel = 0; channel < m_Module->GetChannelCount(); ++channel)
                HandleTick0(channel);
        }
    }

    if (m_CurrentTick != 0)
    {
        for (uint8_t channel = 0; channel < m_Module->GetChannelCount(); ++channel)
            HandleTickX(channel);
    }

    if (m_NextSpeed != m_Speed)
    {
        m_Speed = m_NextSpeed;
        CalculateNextValues(); // Recalculate if necessary.
    }

    if (m_NextBpm != m_Bpm)
    {
        m_Bpm = m_NextBpm;
        m_RefreshRateCallback((m_Bpm << 1) / 5);
    }

    m_CurrentTick = m_NextTick;
    if (m_PatternDelay == 0)
    {
        m_CurrentRow = m_NextRow;
        m_CurrentOrderIndex = m_NextOrderIndex;
        if (m_CurrentOrderIndex >= m_Module->GetOrderCount())
        {
            Stop();
            return;
        }
        CalculateNextValues();
    }
    else
    {
        --m_PatternDelay;
        if ((m_CurrentTick + 1) >= m_Speed)
        {
            m_NextTick = 0;
        }
        else
        {
            m_NextTick = m_CurrentTick + 1;
        }
    }

}

void Player::Pause()
{
    using namespace Ham::Gravis::Shared;

    m_State = State::Paused;
    SYS_Barrier();
    for (uint8_t channel = 0; channel < 32; ++channel)
    {
        Function::System::SetLinearVolume(channel, 0);
        Function::System::StopVoice(channel);
    }
}

void Player::Resume()
{
    using namespace Ham::Gravis::Shared;

    for (uint8_t channel = 0; channel < 32; ++channel)
    {
        Function::System::ResumeVoice(channel);
        Function::System::SetVolume(channel, m_Channels[channel].Volume);
    }
    SYS_Barrier();
    m_State = State::Playing;
}

void Player::Reset()
{
    using namespace Ham::Gravis::Shared;

    m_State = State::Stopped;

    m_Speed = 6;//Default speed
    m_Bpm = 125;//Default beats per minute
    m_RefreshRateCallback((m_Bpm << 1) / 5);
    
    m_PatternDelay = 0;

    m_CurrentTick = 0;
    m_CurrentRow = 0;
    m_CurrentOrderIndex = 0;

    m_NextSpeed = 6;
    m_NextBpm = 125;

    m_NextTick = 1;
    m_NextRow = 0;
    m_NextOrderIndex = 0;

    for (uint8_t i = 0; i < sizeof(m_Channels) / sizeof(Channel); ++i)
    {
        m_Channels[i].SampleBegin = 0;
        m_Channels[i].SampleLoopStart = 0;
        m_Channels[i].SampleLoopEnd = 0;
        m_Channels[i].Period = 0;
        m_Channels[i].PortaTarget = 0;
        m_Channels[i].PortaSpeed = 0;
        m_Channels[i].FineTune = 0;
        m_Channels[i].Balance = ((i & 1) == 0) ? 0x00 : 0x0F;//L, R, L, R, ...
        m_Channels[i].Volume = 0;
        m_Channels[i].Note = 0;
        m_Channels[i].Parameter = 0;
        m_Channels[i].Sample = 0;
        m_Channels[i].LoopTarget = 0;
        m_Channels[i].LoopCounter = 0;

        Function::System::ResetVoice(i);
        Function::System::SetPan(i, m_Channels[i].Balance);
    }
}

void Player::UploadSamples()
{
    using namespace Ham::Gravis::Shared;
    for (uint8_t i = 0; i < m_Module->GetSampleCount(); ++i)
    {
        if (m_Module->GetSampleLength(i) == 0)
            continue;

        uint32_t address = AllocateCardMemory(m_Module->GetSampleLength(i));
        if (address == 0xffffffff)
            return;

        m_Module->SetSampleAddress(i, address);
        Function::System::UploadSound(address, m_Module->GetSampleData(i), m_Module->GetSampleLength(i));
    }
}

uint32_t Player::AllocateCardMemory(uint32_t size)
{
    using namespace Ham::Gravis::Shared;
    for (uint32_t i = 0; i < sizeof(m_MemoryRemaining) / sizeof(uint32_t); ++i)
    {
        if (m_MemoryRemaining[i] >= size)
        {
            uint32_t addr = GF1::Global::DramIOAddress::BankSize - m_MemoryRemaining[i];
            addr |= i << GF1::Global::DramIOAddress::Shift::BankSelect;
            m_MemoryRemaining[i] -= size;
            return addr;
        }
    }
    return 0xffffffff;
}

void Player::HandleTick0(uint8_t channel)
{
    using namespace Has;
    using namespace Ham::File;
    using namespace Ham::Gravis::Shared;

    uint8_t currentOrder = m_Module->GetOrder(m_CurrentOrderIndex);
    Mod::Note* currentPattern = m_Module->GetPattern(currentOrder);
    Mod::Note* currentRow = currentPattern + m_CurrentRow * m_Module->GetChannelCount();
    Mod::Note* currentNote = currentRow + channel;

    // Delay note. We ignore everything until that tick is reached.
    if ((m_CurrentTick == 0) && 
        (currentNote->Effect == Mod::Effect::MoreEffects) &&
        ((currentNote->Parameter & Mod::SubEffect::Mask) == Mod::SubEffect::DelayNote))
        return;

    //property dirty flags:
    bool volumeDirty = false;
    bool panDirty = false;
    bool periodDirty = false;

    if (currentNote->Sample != 0)
    {
        m_Channels[channel].Sample = currentNote->Sample;
        m_Channels[channel].Volume = m_Module->GetSampleVolume(m_Channels[channel].Sample - 1);
        volumeDirty = true;
    }

    if (currentNote->Parameter != 0x00)
        m_Channels[channel].Parameter = currentNote->Parameter;

    if (currentNote->Note != 0xffff)
    {
        m_Channels[channel].Note = currentNote->Note;
        uint8_t sample = m_Channels[channel].Sample - 1;
        uint16_t correctPeriod = m_Module->GetPeriod(currentNote->Note, m_Module->GetSampleFineTune(sample));

        m_Channels[channel].SampleLoopEnd = (m_Module->GetSampleAddress(sample) + m_Module->GetSampleLength(sample)) - 1;
        if (m_Module->GetSampleLoopEnd(sample) != 0)
            m_Channels[channel].SampleLoopEnd = m_Module->GetSampleAddress(sample) + m_Module->GetSampleLoopEnd(sample);

        m_Channels[channel].SampleBegin = m_Module->GetSampleAddress(sample);

        m_Channels[channel].SampleLoopStart = m_Module->GetSampleAddress(sample) + m_Module->GetSampleLoopStart(sample);
        
        //TODO: Process Vibrato and Tremolo

        if ((currentNote->Effect != Mod::Effect::PortaToNote) && (currentNote->Effect != Mod::Effect::PortaAndVolSlide))
        {
            m_Channels[channel].Period = correctPeriod;
            m_Channels[channel].FineTune = m_Module->GetSampleFineTune(sample);
            periodDirty = true;
        }

        if (currentNote->Effect == Mod::Effect::PortaToNote)
        {
            m_Channels[channel].PortaTarget = correctPeriod;
        }
    }

    if ((currentNote->Effect == Mod::Effect::PortaToNote) && (currentNote->Parameter != 0))
    {
        m_Channels[channel].PortaSpeed = currentNote->Parameter;
    }

    if ((currentNote->Effect != 0x00) || (currentNote->Parameter != 0x00))
    {
        switch(currentNote->Effect)
        {
        case Mod::Effect::Pan:// 5.9 Effect 8xy (Pan)
            m_Channels[channel].Balance = min<uint8_t>(currentNote->Parameter >> 3, 0xf);
            //This is supposed to be surround, but that requires 2 channels and an inverted sample.
            if (currentNote->Parameter == 0xA4) m_Channels[channel].Balance = 0x07;
            panDirty = true;
            break;
        case Mod::Effect::SampleOffset:// 5.10 Effect 9xy (Sample Offset)
            {
                uint32_t sampleOffset = ((currentNote->Parameter) != 0 ? currentNote->Parameter : m_Channels[channel].Parameter) << 8;

                m_Channels[channel].SampleBegin = m_Module->GetSampleAddress(m_Channels[channel].Sample - 1) + sampleOffset;

                if (m_Channels[channel].SampleBegin > m_Channels[channel].SampleLoopEnd)
                    m_Channels[channel].SampleBegin = m_Channels[channel].SampleLoopEnd;
            }
            break;
        case Mod::Effect::JumpToPattern:// 5.12 Effect Bxy (Jump To Pattern)
            m_NextOrderIndex = ((currentNote->Parameter & 0xF0) >> 4) * 10 + (currentNote->Parameter & 0x0F);
            m_NextRow = 0;
            if (m_NextOrderIndex >= m_Module->GetOrderCount())
                m_NextOrderIndex = 0;
            break;
        case Mod::Effect::SetVolume:// 5.13 Effect Cxy (Set Volume)
            m_Channels[channel].Volume = min<uint8_t>(currentNote->Parameter, 0x40);
            volumeDirty = true;
            break;
        case Mod::Effect::PatternBreak:// 5.14 Effect Dxy (Pattern Break)
            m_NextOrderIndex = m_CurrentOrderIndex + 1;
            m_NextRow = ((currentNote->Parameter & 0xF0) >> 4) * 10 + (currentNote->Parameter & 0x0F);
            if (m_NextRow > 63)
                m_NextRow = 0;
            break;
        case Mod::Effect::SetSpeed:// 5.15 Effect Fxy (Set Speed)
            if (currentNote->Parameter < 0x20)
            {//Speed
                m_NextSpeed = currentNote->Parameter;
            } else
            {//BPM
                m_NextBpm = currentNote->Parameter;
            }
            break;
        case Mod::Effect::MoreEffects://Sub-effects
            switch(currentNote->Parameter & Mod::SubEffect::Mask)
            {
            case Mod::SubEffect::SetFilter:// 5.16 Effect E0x (Set Filter)
                //Can't implement. Amiga hardware specific.
                break;
            case Mod::SubEffect::FinePortaUp:// 5.17 Effect E1x (Fine Porta Up)
                m_Channels[channel].Period += currentNote->Parameter & 0x0F;
                periodDirty = true;
                break;
            case Mod::SubEffect::FinePortaDown:// 5.18 Effect E2x (Fine Porta Down)
                m_Channels[channel].Period -= currentNote->Parameter & 0x0F;
                periodDirty = true;
                break;
            case Mod::SubEffect::GlissandoControl:// 5.19 Effect E3x (Glissando Control)
                break;
            case Mod::SubEffect::SetVibratoWaveform:// 5.20 Effect E4x (Set Vibrato Waveform)
                break;
            case Mod::SubEffect::SetFinetune:// 5.21 Effect E5x (Set Finetune)
                {
                    m_Channels[channel].FineTune = currentNote->Parameter & 0x0F;
                    if (m_Channels[channel].Note != 0xFFFF)
                    {
                        m_Channels[channel].Period = m_Module->GetPeriod(m_Channels[channel].Note, m_Channels[channel].FineTune);
                        periodDirty = true;
                    }
                }
                break;
            case Mod::SubEffect::PatternLoop:// 5.22 Effect E6x (Pattern Loop)
                if (currentNote->Parameter == 0x00)
                {
                    m_Channels[channel].LoopTarget = m_CurrentRow;
                }
                else
                {
                    if (m_Channels[channel].LoopCounter == 0)
                    {
                        m_Channels[channel].LoopCounter = currentNote->Parameter & 0x0F;
                    }
                    else
                    {
                        --m_Channels[channel].LoopCounter;
                    }
                    if (m_Channels[channel].LoopCounter != 0)
                        m_NextRow = m_Channels[channel].LoopTarget;
                }
                break;
            case Mod::SubEffect::SetTremoloWaveForm:// 5.23 Effect E7x (Set Tremolo WaveForm)
                break;
            case Mod::SubEffect::FinePanning:// 5.24 Effect E8x (16 Position Panning)
                m_Channels[channel].Balance = currentNote->Parameter & 0x0F;
                panDirty = true;
                break;
            case Mod::SubEffect::FineVolumeSlideUp:// 5.26 Effect EAx (Fine Volume Slide Up)
                m_Channels[channel].Volume += currentNote->Parameter & 0x0F;
                volumeDirty = true;
                break;
            case Mod::SubEffect::FineVolumeSlideDown:// 5.27 Effect EBx (Fine Volume Slide Down)
                m_Channels[channel].Volume += currentNote->Parameter & 0x0F;
                volumeDirty = true;
                break;
            case Mod::SubEffect::PatternDelay:// 5.30 Effect EEx (Pattern Delay)
                m_PatternDelay = currentNote->Parameter & 0x0F;
                break;
            case Mod::SubEffect::InvertLoop:// 5.31 Effect EFx (Invert Loop)
                // Not supported in any players or trackers apparently.
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    //Apply channel changes.
    if (periodDirty && (m_Channels[channel].Period > 0))
    {
        uint16_t frequency = uint16_t(7159090.5 / (m_Channels[channel].Period << 1));
        Function::System::SetPlaybackFrequency(channel, frequency, m_Module->GetChannelCount());
    }

    if (volumeDirty)
    {
        m_Channels[channel].Volume = min<int16_t>(max<int16_t>(0, m_Channels[channel].Volume), 64);
        Function::System::SetVolume(channel, s_Volume[m_Channels[channel].Volume]);
    }

    if (panDirty)
        Function::System::SetPan(channel, m_Channels[channel].Balance);

    if (currentNote->Note != 0xffff)
    {
        GF1::Voice::VoiceControl_t voiceControl = GF1::Voice::VoiceControl::Bits8 |
                                                GF1::Voice::VoiceControl::Forward |
                                                GF1::Voice::VoiceControl::Play;

        if (m_Module->GetSampleLoopEnd(m_Channels[channel].Sample - 1) != 0)
            voiceControl |= GF1::Voice::VoiceControl::LoopToBegin;

        Function::System::PlayVoice(channel, m_Channels[channel].SampleBegin, m_Channels[channel].SampleLoopStart, m_Channels[channel].SampleLoopEnd, voiceControl);
    }
}

void Player::HandleTickX(uint8_t channel)
{
    using namespace Has;
    using namespace Ham::File;
    using namespace Ham::Gravis::Shared;

    uint8_t currentOrder = m_Module->GetOrder(m_CurrentOrderIndex);
    Mod::Note* currentPattern = m_Module->GetPattern(currentOrder);
    Mod::Note* currentRow = currentPattern + m_CurrentRow * m_Module->GetChannelCount();
    Mod::Note* currentNote = currentRow + channel;

    //property dirty flags:
    bool volumeDirty = false;
    bool periodDirty = false;

    switch(currentNote->Effect)
    {
    case Mod::Effect::Arpeggio:// 5.1 Effect 0xy (Arpeggio)
        break;
    case Mod::Effect::PortaUp:// 5.2 Effect 1xy (Porta Up)
        m_Channels[channel].Period = max<uint16_t>(m_Channels[channel].Period - currentNote->Parameter, 56);//TODO don't harcode this!
        periodDirty = true;
        break;
    case Mod::Effect::PortaDown:// 5.3 Effect 2xy (Porta Down)
        m_Channels[channel].Period = min<uint16_t>(m_Channels[channel].Period + currentNote->Parameter, 0x6B0);//TODO don't harcode this!
        periodDirty = true;
        break;
    case Mod::Effect::PortaToNote:// 5.4 Effect 3xy (Porta To Note)
        if (m_Channels[channel].Period != m_Channels[channel].PortaTarget)
        {
            if (m_Channels[channel].Period < m_Channels[channel].PortaTarget)
            {
                m_Channels[channel].Period = min<uint16_t>(m_Channels[channel].Period + m_Channels[channel].PortaSpeed, m_Channels[channel].PortaTarget);
            }
            else
            {
                m_Channels[channel].Period = max<uint16_t>(m_Channels[channel].Period - m_Channels[channel].PortaSpeed, m_Channels[channel].PortaTarget);
            }
        }
        break;
    case Mod::Effect::Vibrato:// 5.5 Effect 4xy (Vibrato)
        break;
    case Mod::Effect::PortaAndVolSlide:// 5.6 Effect 5xy (Porta + Vol Slide)
        break;
    case Mod::Effect::VibratoAndVolSlide:// 5.7 Effect 6xy (Vibrato + Vol Slide)
        break;
    case Mod::Effect::Tremolo:// 5.8 Effect 7xy (Tremolo)
        break;
    case Mod::Effect::VolumeSlide:// 5.11 Effect Axy (Volume Slide)
        if (((currentNote->Parameter & 0xF0) != 0) && ((currentNote->Parameter & 0x0F) == 0))
        {
            //up
            m_Channels[channel].Volume += (currentNote->Parameter & 0xF0) >> 4;
            volumeDirty = true;
        }
        else if (((currentNote->Parameter & 0x0F) != 0) && ((currentNote->Parameter & 0xF0) == 0))
        {
            //down
            m_Channels[channel].Volume -= currentNote->Parameter & 0x0F;
            volumeDirty = true;
        }
        break;
    case Mod::Effect::MoreEffects://Sub-effects
        switch (currentNote->Parameter & Mod::SubEffect::Mask)
        {
        case Mod::SubEffect::RetriggerNote:// 5.25 Effect E9x (Retrig Note)
            if ((m_CurrentTick % (currentNote->Parameter & 0x0F)) == 0)
            {
                if (m_Channels[channel].Note != 0xffff)
                {
                    GF1::Voice::VoiceControl_t voiceControl = GF1::Voice::VoiceControl::Bits8 |
                                                            GF1::Voice::VoiceControl::Forward |
                                                            GF1::Voice::VoiceControl::Play;

                    if (m_Module->GetSampleLoopEnd(m_Channels[channel].Sample - 1) != 0)
                        voiceControl |= GF1::Voice::VoiceControl::LoopToBegin;

                    Function::System::PlayVoice(channel, m_Channels[channel].SampleBegin, m_Channels[channel].SampleLoopStart, m_Channels[channel].SampleLoopEnd, voiceControl);
                }
            }
            break;
        case Mod::SubEffect::CutNote:// 5.28 Effect ECx (Cut Note)
            if (m_CurrentTick == (currentNote->Parameter & 0x0F)) m_Channels[channel].Volume = 0;
            volumeDirty = true;
            break;
        case Mod::SubEffect::DelayNote:// 5.29 Effect EDx (Delay Note)
            if (m_CurrentTick == (currentNote->Parameter & 0x0F)) HandleTick0(channel); // Now we trigger the note.
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    //Apply channel changes.
    if (periodDirty && (m_Channels[channel].Period > 0))
    {
        uint16_t frequency = uint16_t(7159090.5 / (m_Channels[channel].Period << 1));
        Function::System::SetPlaybackFrequency(channel, frequency, m_Module->GetChannelCount());
    }

    if (volumeDirty)
    {
        m_Channels[channel].Volume = min<int16_t>(max<int16_t>(0, m_Channels[channel].Volume), 64);
        Function::System::SetVolume(channel, s_Volume[m_Channels[channel].Volume]);
    }
}

uint16_t Player::s_Volume[65] =
{
    01500,
    0xA0DE, 0xAB52, 0xB2BD, 0xB87E, 0xBD31, 0xC12B, 0xC49C, 0xC7A5,
    0xCA5D, 0xCCD2, 0xCF10, 0xD120, 0xD309, 0xD4D1, 0xD67B, 0xD80B,
    0xD984, 0xDAE9, 0xDC3B, 0xDD7D, 0xDEB0, 0xDFD6, 0xE0EF, 0xE1FC,
    0xE2FF, 0xE3F8, 0xE4E8, 0xE5D0, 0xE6AF, 0xE788, 0xE859, 0xE924,
    0xE9E9, 0xEAA9, 0xEB63, 0xEC18, 0xECC8, 0xED73, 0xEE1A, 0xEEBD,
    0xEF5C, 0xEFF7, 0xF08F, 0xF123, 0xF1B5, 0xF242, 0xF2CD, 0xF356,
    0xF3DB, 0xF45E, 0xF4DE, 0xF55B, 0xF5D7, 0xF650, 0xF6C7, 0xF73C,
    0xF7AE, 0xF81F, 0xF88E, 0xF8FB, 0xF967, 0xF9D0, 0xFA38, 0xFA9E
};

}