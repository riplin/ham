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
        if (m_PatternDelay != 0)
            --m_PatternDelay;

        if (m_PatternDelay == 0)
        {
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
    m_CurrentRow = m_NextRow;
    m_CurrentOrderIndex = m_NextOrderIndex;
    if (m_CurrentOrderIndex >= m_Module->GetOrderCount())
    {
        Stop();
        return;
    }
    CalculateNextValues();
}

void Player::Pause()
{
    using namespace Ham::Gravis::Shared;

    m_State = State::Paused;
    SYS_Barrier();
    for (uint8_t channel = 0; channel < 32; ++channel)
    {
        Function::System::SetVolume(channel, s_Volume[0]);
        Function::System::StopVoice(channel);
    }
}

void Player::Resume()
{
    using namespace Ham::Gravis::Shared;

    for (uint8_t channel = 0; channel < 32; ++channel)
    {
        Function::System::ResumeVoice(channel);
        Function::System::SetVolume(channel, s_Volume[m_Channels[channel].Volume]);
        m_Channels[channel].Mute = false;
    }
    SYS_Barrier();
    m_State = State::Playing;
}

void Player::ToggleChannelMute(uint8_t channel)
{
    using namespace Gravis::Shared;

    m_Channels[channel].Mute = !m_Channels[channel].Mute;
    if (m_Channels[channel].Mute)
        Function::System::SetVolume(channel, s_Volume[0]);
    else
        Function::System::SetVolume(channel, s_Volume[m_Channels[channel].Volume]);
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
        m_Channels[i].Volume = 0;
        m_Channels[i].VolumeTarget = 0;
        m_Channels[i].VolumeDelta = 0;
        m_Channels[i].Note = 0;
        m_Channels[i].PortaSpeed = 0;
        m_Channels[i].SampleOffset = 0;
        m_Channels[i].FineTune = 0;
        m_Channels[i].Balance = ((i & 1) == 0) ? 0x03 : 0x0C;//L, R, L, R, ...
        m_Channels[i].Parameter = 0;
        m_Channels[i].Sample = 0;
        m_Channels[i].LoopTarget = 0;
        m_Channels[i].LoopCounter = 0;
        m_Channels[i].VibratoDepth = 0;
        m_Channels[i].VibratoSpeed = 0;
        m_Channels[i].VibratoPosition = 0;
        m_Channels[i].VibratoWaveType = WaveType::Sine;
        m_Channels[i].TremoloDepth = 0;
        m_Channels[i].TremoloSpeed = 0;
        m_Channels[i].TremoloPosition = 0;
        m_Channels[i].TremoloWaveType = WaveType::Sine;
        m_Channels[i].Mute = false;

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
    using namespace Has;
    using namespace Ham::Gravis::Shared;

    size = alignup<uint32_t>(size, 32);
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
    bool sampleDirty = false;
    bool panDirty = false;
    bool periodDirty = false;

    if (currentNote->Sample != 0)
    {
        m_Channels[channel].Sample = currentNote->Sample;
        m_Channels[channel].FineTune = m_Module->GetSampleFineTune(m_Channels[channel].Sample - 1);
        m_Channels[channel].VolumeTarget = m_Module->GetSampleVolume(m_Channels[channel].Sample - 1);
    }

    if (currentNote->Parameter != 0x00)
        m_Channels[channel].Parameter = currentNote->Parameter;

    uint16_t correctPeriod = 0;
    if (currentNote->Note != 0xffff)
    {
        m_Channels[channel].Note = currentNote->Note;
        uint8_t sample = m_Channels[channel].Sample - 1;
        correctPeriod = m_Module->GetPeriod(currentNote->Note, m_Channels[channel].FineTune);

        m_Channels[channel].SampleLoopEnd = (m_Module->GetSampleAddress(sample) + m_Module->GetSampleLength(sample));
        m_Channels[channel].SampleBegin = m_Module->GetSampleAddress(sample);
        m_Channels[channel].SampleLoopStart = m_Module->GetSampleAddress(sample) + m_Module->GetSampleLoopStart(sample);
        
        if ((m_Channels[channel].VibratoWaveType & WaveType::DontRetrigger) == 0)
            m_Channels[channel].VibratoPosition = 0;

        if ((m_Channels[channel].TremoloWaveType & WaveType::DontRetrigger) == 0)
            m_Channels[channel].TremoloPosition = 0;

        if ((currentNote->Effect != Mod::Effect::PortamentoToNote) && (currentNote->Effect != Mod::Effect::PortamentoAndVolumeSlide))
        {
            m_Channels[channel].Period = correctPeriod;
            sampleDirty = true;
            periodDirty = true;
        }
    }

    if ((currentNote->Effect != 0x00) || (currentNote->Parameter != 0x00))
    {
        switch(currentNote->Effect)
        {
        case Mod::Effect::PortamentoToNote:// 5.4 Effect 3xy (Porta To Note)
            if (correctPeriod != 0)
            {
                m_Channels[channel].PortaTarget = correctPeriod;
            }
            if (currentNote->Parameter != 0)
            {
                m_Channels[channel].PortaSpeed = currentNote->Parameter;
            }
            break;
        case Mod::Effect::Vibrato:// 5.5 Effect 4xy (Vibrato)
            if ((currentNote->Parameter & 0xF0) != 0)
                m_Channels[channel].VibratoSpeed = (currentNote->Parameter & 0xF0) >> 4;
            if ((currentNote->Parameter & 0x0F) != 0)
                m_Channels[channel].VibratoDepth = currentNote->Parameter & 0x0F;
            break;
        case Mod::Effect::PortamentoAndVolumeSlide:// 5.6 Effect 5xy (Porta + Vol Slide)
            if (correctPeriod != 0)
            {
                m_Channels[channel].PortaTarget = correctPeriod;
            }
            break;
        case Mod::Effect::Tremolo:// 5.8 Effect 7xy (Tremolo)
            if ((currentNote->Parameter & 0xF0) != 0)
                m_Channels[channel].TremoloSpeed = (currentNote->Parameter & 0xF0) >> 4;
            if ((currentNote->Parameter & 0x0F) != 0)
                m_Channels[channel].TremoloDepth = currentNote->Parameter & 0x0F;
            break;
        case Mod::Effect::Pan:// 5.9 Effect 8xy (Pan)
            m_Channels[channel].Balance = min<uint8_t>(currentNote->Parameter >> 3, 0xf);
            //This is supposed to be surround, but that requires 2 channels and an inverted sample.
            if (currentNote->Parameter == 0xA4) m_Channels[channel].Balance = 0x07;
            panDirty = true;
            break;
        case Mod::Effect::SampleOffset:// 5.10 Effect 9xy (Sample Offset)
            {
                m_Channels[channel].SampleOffset = (currentNote->Parameter) != 0 ? currentNote->Parameter : m_Channels[channel].SampleOffset;
                uint32_t sampleOffset = uint32_t(m_Channels[channel].SampleOffset) << 8;

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
            m_Channels[channel].VolumeTarget = min<uint8_t>(currentNote->Parameter, 0x40);
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
                m_Channels[channel].Period -= currentNote->Parameter & 0x0F;
                periodDirty = true;
                break;
            case Mod::SubEffect::FinePortaDown:// 5.18 Effect E2x (Fine Porta Down)
                m_Channels[channel].Period += currentNote->Parameter & 0x0F;
                periodDirty = true;
                break;
            case Mod::SubEffect::GlissandoControl:// 5.19 Effect E3x (Glissando Control)
                break;
            case Mod::SubEffect::SetVibratoWaveform:// 5.20 Effect E4x (Set Vibrato Waveform)
                m_Channels[channel].VibratoWaveType = WaveType(currentNote->Parameter & 0x07);
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
                m_Channels[channel].TremoloWaveType = WaveType(currentNote->Parameter & 0x07);
                break;
            case Mod::SubEffect::FinePanning:// 5.24 Effect E8x (16 Position Panning)
                m_Channels[channel].Balance = currentNote->Parameter & 0x0F;
                panDirty = true;
                break;
            case Mod::SubEffect::FineVolumeSlideUp:// 5.26 Effect EAx (Fine Volume Slide Up)
                m_Channels[channel].VolumeTarget += currentNote->Parameter & 0x0F;
                break;
            case Mod::SubEffect::FineVolumeSlideDown:// 5.27 Effect EBx (Fine Volume Slide Down)
                m_Channels[channel].VolumeTarget += currentNote->Parameter & 0x0F;
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

    if (sampleDirty)
    {
        Function::System::StopVoice(channel);
    }

    //Apply channel changes.
    if (periodDirty && (m_Channels[channel].Period > 0))
    {
        uint16_t frequency = uint16_t(7159090.5 / (m_Channels[channel].Period << 1));
        Function::System::SetPlaybackFrequency(channel, frequency, m_Module->GetChannelCount());
    }

    ProcessVolume(channel, 0);

    if (panDirty)
        Function::System::SetPan(channel, m_Channels[channel].Balance);

    if (sampleDirty)
    {
        GF1::Voice::VoiceControl_t voiceControl = GF1::Voice::VoiceControl::Bits8 |
                                                GF1::Voice::VoiceControl::Forward |
                                                GF1::Voice::VoiceControl::Play;

        if (m_Module->GetSampleLoopEnd(m_Channels[channel].Sample - 1) != 0)
            voiceControl |= GF1::Voice::VoiceControl::LoopToBegin;

        Function::System::PlayVoice(channel, m_Channels[channel].SampleBegin, m_Channels[channel].SampleLoopStart, m_Channels[channel].SampleLoopEnd, voiceControl);
    }
}

int16_t Player::ProcessArpeggio(uint8_t channel, uint8_t parameter)
{
    int16_t delta = 0;
    switch (m_CurrentTick % 3)
    {
    case 0:
        //base note.
        break;
    case 1:
        delta = m_Module->GetPeriod(m_Channels[channel].Note + (parameter >> 4), m_Channels[channel].FineTune) - m_Channels[channel].Period;
        break;
    case 2:
        delta = m_Module->GetPeriod(m_Channels[channel].Note + (parameter & 0x0F), m_Channels[channel].FineTune) - m_Channels[channel].Period;
        break;
    }
    return delta;
}

bool Player::ProcessPortamentoToNote(uint8_t channel)
{
    using namespace Has;
    bool periodDirty = false;
    if (m_Channels[channel].Period != m_Channels[channel].PortaTarget)
    {
        if (m_Channels[channel].Period < m_Channels[channel].PortaTarget)
        {
            uint16_t newPeriod = m_Channels[channel].Period + m_Channels[channel].PortaSpeed;
            m_Channels[channel].Period = min<uint16_t>(newPeriod, m_Channels[channel].PortaTarget);
        }
        else
        {
            int32_t newPeriod = int32_t(m_Channels[channel].Period) - int32_t(m_Channels[channel].PortaSpeed);
            m_Channels[channel].Period = uint16_t(max<int32_t>(newPeriod, int32_t(m_Channels[channel].PortaTarget)));
        }
        periodDirty = true;
    }
    return periodDirty;
}

void Player::ProcessVolumeSlide(uint8_t channel, uint8_t parameter)
{
    if (((parameter & 0xF0) != 0) && ((parameter & 0x0F) == 0))
    {
        //up
        m_Channels[channel].VolumeTarget += (parameter & 0xF0) >> 4;
    }
    else if (((parameter & 0x0F) != 0) && ((parameter & 0xF0) == 0))
    {
        //down
        m_Channels[channel].VolumeTarget -= parameter & 0x0F;
    }
}

int16_t Player::ProcessVibrato(uint8_t channel)
{
    uint8_t pos = m_Channels[channel].VibratoPosition & 0x31;

    int16_t delta = 0;
    switch(m_Channels[channel].VibratoWaveType & WaveType::Type)
    {
    case WaveType::Sine:
        delta = s_SineTable[pos];
        break;
    case WaveType::SawTooth:
        pos <<= 3;
        if (m_Channels[channel].VibratoPosition < 0)
            pos = 0xFF - pos;
        delta = pos;
        break;
    case WaveType::Square:
        delta = 0xFF;
        break;
    case WaveType::Random:
        delta = s_SineTable[pos];
        break;
    }

    delta *= m_Channels[channel].VibratoDepth;
    delta >>= 7;

    if (m_Channels[channel].VibratoPosition < 0)
        delta = -delta;

    m_Channels[channel].VibratoPosition += m_Channels[channel].VibratoSpeed;

    if (m_Channels[channel].VibratoPosition > 31)
        m_Channels[channel].VibratoPosition -= 64;

    return delta;
}

int16_t Player::ProcessTremolo(uint8_t channel)
{
    uint8_t pos = m_Channels[channel].TremoloPosition & 0x31;

    int16_t delta = 0;
    switch(m_Channels[channel].TremoloWaveType & WaveType::Type)
    {
    case WaveType::Sine:
        delta = s_SineTable[pos];
        break;
    case WaveType::SawTooth:
        pos <<= 3;
        if (m_Channels[channel].TremoloPosition < 0)
            pos = 0xFF - pos;
        delta = pos;
        break;
    case WaveType::Square:
        delta = 0xFF;
        break;
    case WaveType::Random:
        delta = s_SineTable[pos];
        break;
    }

    delta *= m_Channels[channel].TremoloDepth;
    delta >>= 7;

    if (m_Channels[channel].TremoloPosition < 0)
        delta = -delta;

    m_Channels[channel].TremoloPosition += m_Channels[channel].TremoloSpeed;

    if (m_Channels[channel].TremoloPosition > 31)
        m_Channels[channel].TremoloPosition -= 64;

    return delta;
}

void Player::ProcessVolume(uint8_t channel, int16_t volumeDelta)
{
    using namespace Has;
    using namespace Ham::Gravis::Shared;

    if ((m_Channels[channel].Volume + m_Channels[channel].VolumeDelta) != (m_Channels[channel].VolumeTarget + volumeDelta))
    {
        m_Channels[channel].VolumeTarget = min<int16_t>(max<int16_t>(0, m_Channels[channel].VolumeTarget), 64);
        
        int16_t toVolume = min<int16_t>(max<int16_t>(0, m_Channels[channel].VolumeTarget + volumeDelta), 64);

        //TODO: Was for ramping, but that doesn't sound right...
        //int16_t fromVolume = min<int16_t>(max<int16_t>(0, m_Channels[channel].Volume + m_Channels[channel].VolumeDelta), 64);

        if (m_Channels[channel].Mute)
        {
            Function::System::SetVolume(channel, s_Volume[0]);
        }
        else
        {
            Function::System::SetVolume(channel, s_Volume[toVolume]);
            m_Channels[channel].Volume = m_Channels[channel].VolumeTarget;
            m_Channels[channel].VolumeDelta = volumeDelta;
        }
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
    bool periodDirty = false;
    int16_t periodDelta = 0;
    int16_t volumeDelta = 0;

    switch(currentNote->Effect)
    {
    case Mod::Effect::Arpeggio:// 5.1 Effect 0xy (Arpeggio)
        periodDelta = ProcessArpeggio(channel, currentNote->Parameter);
        periodDirty = true;
        break;
    case Mod::Effect::PortamentoUp:// 5.2 Effect 1xy (Porta Up)
        m_Channels[channel].Period = max<int16_t>(m_Channels[channel].Period - int16_t(currentNote->Parameter), 56);//TODO don't harcode this!
        periodDirty = true;
        break;
    case Mod::Effect::PortamentoDown:// 5.3 Effect 2xy (Porta Down)
        m_Channels[channel].Period = min<int16_t>(m_Channels[channel].Period + int16_t(currentNote->Parameter), 0x6B0);//TODO don't harcode this!
        periodDirty = true;
        break;
    case Mod::Effect::PortamentoToNote:// 5.4 Effect 3xy (Porta To Note)
        periodDirty = ProcessPortamentoToNote(channel);
        break;
    case Mod::Effect::Vibrato:// 5.5 Effect 4xy (Vibrato)
        periodDelta = ProcessVibrato(channel);
        periodDirty = true;
        break;
    case Mod::Effect::PortamentoAndVolumeSlide:// 5.6 Effect 5xy (Porta + Vol Slide)
        ProcessVolumeSlide(channel, currentNote->Parameter);
        periodDirty = ProcessPortamentoToNote(channel);
        break;
    case Mod::Effect::VibratoAndVolumeSlide:// 5.7 Effect 6xy (Vibrato + Vol Slide)
        ProcessVolumeSlide(channel, currentNote->Parameter);
        periodDelta = ProcessVibrato(channel);
        periodDirty = true;
        break;
    case Mod::Effect::Tremolo:// 5.8 Effect 7xy (Tremolo)
        volumeDelta = ProcessTremolo(channel);
        break;
    case Mod::Effect::VolumeSlide:// 5.11 Effect Axy (Volume Slide)
        ProcessVolumeSlide(channel, currentNote->Parameter);
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
            if (m_CurrentTick == (currentNote->Parameter & 0x0F)) m_Channels[channel].VolumeTarget = 0;
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
        uint16_t frequency = uint16_t(7159090.5 / ((m_Channels[channel].Period + periodDelta) << 1));
        Function::System::SetPlaybackFrequency(channel, frequency, m_Module->GetChannelCount());
    }

    ProcessVolume(channel, volumeDelta);
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

uint8_t Player::s_SineTable[32] =
{
      0,  24,  49,  74,  97, 120, 141, 161,
    180, 197, 212, 224, 235, 244, 250, 253,
    255, 253, 250, 244, 235, 224, 212, 197,
    180, 161, 141, 120,  97,  74,  49,  24
};

}