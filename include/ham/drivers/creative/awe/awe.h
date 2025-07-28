//Copyright 2025-Present riplin

#pragma once

#include <has/imports.h>

#include <ham/drivers/creative/shared/emu8k/data.h>
#include <ham/drivers/creative/shared/emu8k/index.h>

#include <ham/drivers/creative/shared/emu8k/global/hwcfg.h>
#include <ham/drivers/creative/shared/emu8k/global/smar.h>
#include <ham/drivers/creative/shared/emu8k/global/smaw.h>
#include <ham/drivers/creative/shared/emu8k/global/smd.h>
#include <ham/drivers/creative/shared/emu8k/global/wc.h>

#include <ham/drivers/creative/shared/emu8k/voice/atkhld.h>
#include <ham/drivers/creative/shared/emu8k/voice/atkhldv.h>
#include <ham/drivers/creative/shared/emu8k/voice/ccca.h>
#include <ham/drivers/creative/shared/emu8k/voice/cpf.h>
#include <ham/drivers/creative/shared/emu8k/voice/csl.h>
#include <ham/drivers/creative/shared/emu8k/voice/cvcf.h>
#include <ham/drivers/creative/shared/emu8k/voice/dcysus.h>
#include <ham/drivers/creative/shared/emu8k/voice/dcysusv.h>
#include <ham/drivers/creative/shared/emu8k/voice/envval.h>
#include <ham/drivers/creative/shared/emu8k/voice/envvol.h>
#include <ham/drivers/creative/shared/emu8k/voice/fm2frq2.h>
#include <ham/drivers/creative/shared/emu8k/voice/fmmod.h>
#include <ham/drivers/creative/shared/emu8k/voice/ifatn.h>
#include <ham/drivers/creative/shared/emu8k/voice/init.h>
#include <ham/drivers/creative/shared/emu8k/voice/ip.h>
#include <ham/drivers/creative/shared/emu8k/voice/lfoval.h>
#include <ham/drivers/creative/shared/emu8k/voice/pefe.h>
#include <ham/drivers/creative/shared/emu8k/voice/psst.h>
#include <ham/drivers/creative/shared/emu8k/voice/ptrx.h>
#include <ham/drivers/creative/shared/emu8k/voice/tremfrq.h>
#include <ham/drivers/creative/shared/emu8k/voice/vtft.h>

namespace Ham::Creative::Awe
{

typedef Shared::Emu8k::Data16_t Data16_t;
typedef Shared::Emu8k::Data32_t Data32_t;
IMPORTNAMESPACE(Shared::Emu8k, Data);
IMPORTNAMESPACEANDTYPE(Shared::Emu8k, Index);

namespace Global
{
    typedef Shared::Emu8k::Global::Configuration16_t Configuration16_t;
    typedef Shared::Emu8k::Global::Configuration32_t Configuration32_t;
    typedef Shared::Emu8k::Global::SoundMemoryAddress_t SoundMemoryAddress_t;
    IMPORTNAMESPACE(Shared::Emu8k::Global, Configuration);
    IMPORTNAMESPACE(Shared::Emu8k::Global, SoundMemoryAddressRead);
    IMPORTNAMESPACE(Shared::Emu8k::Global, SoundMemoryAddressWrite);
    IMPORTNAMESPACEANDTYPE(Shared::Emu8k::Global, SoundMemoryData);
    IMPORTNAMESPACEANDTYPE(Shared::Emu8k::Global, SampleCounter);
}

namespace Voice
{
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, ModulationEnvelopeHoldAndAttack);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, VolumeEnvelopeHoldAndAttack);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, QControlBitsAndCurrentAddress);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, CurrentPitchAndFractionalAddress);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, ChorusSendAndLoopEndAddress);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, CurrentVolumeAndFilterCutoff);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, ModulationEnvelopeSustainAndDecay);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, VolumeEnvelopeSustainAndDecay);
    IMPORTNAMESPACEANDTYPE(Shared::Emu8k::Voice, ModulationEnvelopeDelay);
    IMPORTNAMESPACEANDTYPE(Shared::Emu8k::Voice, VolumeEnvelopeDelay);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, Lfo2VibratoAmountAndFrequency);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, VibratoAndFilterModulationFromLfo1);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, InitialFilterCutoffAndAttenuation);
    IMPORTNAMESPACEANDTYPE(Shared::Emu8k::Voice, Initialization);
    IMPORTNAMESPACEANDTYPE(Shared::Emu8k::Voice, InitialPitch);
    IMPORTNAMESPACEANDTYPE(Shared::Emu8k::Voice, LfoDelay);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, PitchAndFilterEnvelopeHeights);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, PanSendAndLoopStartAddress);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, PitchTargetReverbSendAndAux);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, Lfo1TremoloAmountAndFrequency);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Emu8k::Voice, VolumeAndFilterCutoffTargets);
}

}

// Name    Reg #   Cnl #   I/O Port    Data Size   Function
// CPF     0       all     Data 0      DW          Current Pitch and Fractional Address[cnl]
// PTRX    1       all     Data 0      DW          Pitch Target, Rvb Send, and Aux Byte[cnl]
// CVCF    2       all     Data 0      DW          Current Volume and Filter Cutoff [cnl]
// VTFT    3       all     Data 0      DW          Volume and Filter Cutoff Targets [cnl]
// PSST    6       all     Data 0      DW          Pan Send and Loop Start Address [cnl]
// CSL     7       all     Data 0      DW          Chorus Send and Loop End Address [cnl]
// CCCA    0       all     Data 1      DW          Q, Control Bits, and Current Address[cnl]
// HWCF4   1       9       Data 1      DW          Configuration DoubleWord 4
// HWCF5   1       10      Data 1      DW          Configuration DoubleWord 5
// HWCF6   1       13      Data 1      DW          Configuration DoubleWord 6
// SMALR   1       20      Data 1      DW          Sound Memory Address for “Left” SM Reads
// SMARR   1       21      Data 1      DW          Sound Memory Address for “Right” SM Reads
// SMALW   1       22      Data 1      DW          Sound Memory Address for “Left” SM Writes
// SMARW   1       23      Data 1      DW          Sound Memory Address for “Right” SM Writes
// SMLD    1       26      Data 1      Word        Sound Memory “Left” Data
// SMRD    1       26      Data 2      Word        Sound Memory “Right” Data
// WC      1       27      Data 2      Word        Sample Counter
// HWCF1   1       29      Data 1      Word        Configuration Word 1
// HWCF2   1       30      Data 1      Word        Configuration Word 2
// HWCF3   1       31      Data 1      Word        Configuration Word 3
// INIT1   2       all     Data 1      Word        Initialization Array 1
// INIT2   2       all     Data 2      Word        Initialization Array 2
// INIT3   3       all     Data 1      Word        Initialization Array 3
// INIT4   3       all     Data 2      Word        Initialization Array 4
// ENVVOL  4       all     Data 1      Word        Volume Envelope Delay [cnl]
// DCYSUSV 5       all     Data 1      Word        Volume Envelope Sustain and Decay [cnl]
// ENVVAL  6       all     Data 1      Word        Modulation Envelope Delay [cnl]
// DCYSUS  7       all     Data 1      Word        Modulation Envelope Sustain and Decay [cnl]
// ATKHLDV 4       all     Data 2      Word        Volume Envelope Hold and Attack [cnl]
// LFO1VAL 5       all     Data 2      Word        LFO #1 Delay [cnl]
// ATKHLD  6       all     Data 2      Word        Modulation Envelope Hold and Attack [cnl]
// LFO2VAL 7       all     Data 2      Word        LFO #2 Delay [cnl]
// IP      0       all     Data 3      Word        Initial Pitch [cnl]
// IFATN   1       all     Data 3      Word        Initial Filter Cutoff and Attenuation [cnl]
// PEFE    2       all     Data 3      Word        Pitch and Filter Envelope Heights [cnl]
// FMMOD   3       all     Data 3      Word        Vibrato and Filter Modulation from LFO #1[cnl]
// TREMFRQ 4       all     Data 3      Word        LFO #1 Tremolo Amount and Frequency [cnl]
// FM2FRQ2 5       all     Data 3      Word        LFO #2 Vibrato Amount and Frequency [cnl]
