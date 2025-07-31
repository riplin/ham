//Copyright 2025-Present riplin

#pragma once

#include <has/imports.h>

#include <ham/drivers/gravis/shared/system.h>

#include <ham/drivers/gravis/shared/gf1/data.h>
#include <ham/drivers/gravis/shared/gf1/dmactrl.h>
#include <ham/drivers/gravis/shared/gf1/dramio.h>
#include <ham/drivers/gravis/shared/gf1/index.h>
#include <ham/drivers/gravis/shared/gf1/intrctrl.h>
#include <ham/drivers/gravis/shared/gf1/intrstat.h>
#include <ham/drivers/gravis/shared/gf1/mixcrtl.h>
#include <ham/drivers/gravis/shared/gf1/page.h>
#include <ham/drivers/gravis/shared/gf1/regctrl.h>
#include <ham/drivers/gravis/shared/gf1/revlvl.h>
#include <ham/drivers/gravis/shared/gf1/tmrctrl.h>
#include <ham/drivers/gravis/shared/gf1/tmrdata.h>

#include <ham/drivers/gravis/shared/gf1/global/dmastadd.h>
#include <ham/drivers/gravis/shared/gf1/global/dramdma.h>
#include <ham/drivers/gravis/shared/gf1/global/dramioad.h>
#include <ham/drivers/gravis/shared/gf1/global/jstrdac.h>
#include <ham/drivers/gravis/shared/gf1/global/reset.h>
#include <ham/drivers/gravis/shared/gf1/global/smpctrl.h>
#include <ham/drivers/gravis/shared/gf1/global/smpfrq.h>
#include <ham/drivers/gravis/shared/gf1/global/tmrcnt.h>
#include <ham/drivers/gravis/shared/gf1/global/tmrctrl.h>

#include <ham/drivers/gravis/shared/gf1/voice/actvvoc.h>
#include <ham/drivers/gravis/shared/gf1/voice/curloc.h>
#include <ham/drivers/gravis/shared/gf1/voice/curvol.h>
#include <ham/drivers/gravis/shared/gf1/voice/endloc.h>
#include <ham/drivers/gravis/shared/gf1/voice/frqctrl.h>
#include <ham/drivers/gravis/shared/gf1/voice/intrsrc.h>
#include <ham/drivers/gravis/shared/gf1/voice/panpos.h>
#include <ham/drivers/gravis/shared/gf1/voice/startloc.h>
#include <ham/drivers/gravis/shared/gf1/voice/vocctrl.h>
#include <ham/drivers/gravis/shared/gf1/voice/volrmp.h>
#include <ham/drivers/gravis/shared/gf1/voice/volrmpct.h>
#include <ham/drivers/gravis/shared/gf1/voice/volrmprt.h>

#include <ham/drivers/gravis/shared/midi/control.h>
#include <ham/drivers/gravis/shared/midi/data.h>
#include <ham/drivers/gravis/shared/midi/status.h>

#include <ham/drivers/gravis/shared/codec/capt.h>
#include <ham/drivers/gravis/shared/codec/data.h>
#include <ham/drivers/gravis/shared/codec/index.h>
#include <ham/drivers/gravis/shared/codec/play.h>
#include <ham/drivers/gravis/shared/codec/status.h>

#include <ham/drivers/gravis/shared/codec/auxic.h>
#include <ham/drivers/gravis/shared/codec/df.h>
#include <ham/drivers/gravis/shared/codec/ic.h>
#include <ham/drivers/gravis/shared/codec/itfconf.h>
#include <ham/drivers/gravis/shared/codec/lc.h>
#include <ham/drivers/gravis/shared/codec/modeid.h>
#include <ham/drivers/gravis/shared/codec/oc.h>
#include <ham/drivers/gravis/shared/codec/pbc.h>
#include <ham/drivers/gravis/shared/codec/pc.h>
#include <ham/drivers/gravis/shared/codec/tstinit.h>

#include <ham/drivers/gravis/shared/codec/afei.h>
#include <ham/drivers/gravis/shared/codec/afeii.h>
#include <ham/drivers/gravis/shared/codec/afs.h>
#include <ham/drivers/gravis/shared/codec/cbc.h>
#include <ham/drivers/gravis/shared/codec/cdf.h>
#include <ham/drivers/gravis/shared/codec/lic.h>
#include <ham/drivers/gravis/shared/codec/mioc.h>
#include <ham/drivers/gravis/shared/codec/timer.h>
#include <ham/drivers/gravis/shared/codec/vcid.h>

namespace Ham::Gravis::Max
{

    HAM_GRAVIS_SYSTEM;

    IMPORTNAMESPACE(Shared::GF1, Data);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1, DmaControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1, DramIO);
    IMPORTNAMESPACEANDTYPE(Shared::GF1, Index);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1, InterruptControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1, InterruptStatus);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1, MixControl);
    IMPORTNAMESPACEANDTYPE(Shared::GF1, Page);
    IMPORTNAMESPACEANDTYPE(Shared::GF1, RegisterControl);
    IMPORTNAMESPACEANDTYPE(Shared::GF1, RevisionLevel);
    IMPORTNAMESPACEANDTYPE(Shared::GF1, TimerControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1, TimerData);

    namespace Global
    {
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Global, DmaStartAddress);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Global, DramDmaControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Global, DramIOAddress);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Global, JoyStickTrimDac);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Global, Reset);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Global, SamplingControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Global, SamplingFrequency);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Global, TimerCount);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Global, TimerControl);
    }

    namespace Voice
    {
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Voice, ActiveVoices);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Voice, CurrentLocation);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Voice, CurrentVolume);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Voice, EndLocation);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Voice, FrequencyControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Voice, InterruptSource);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Voice, PanPosition);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Voice, StartLocation);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Voice, VoiceControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Voice, VolumeRamp);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Voice, VolumeRampControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::GF1::Voice, VolumeRampRate);
    }

    namespace Midi
    {
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Midi, Control);
        IMPORTNAMESPACEANDTYPE(Shared::Midi, Data);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Midi, Status);
    }

    namespace Codec
    {
        IMPORTNAMESPACEANDTYPE(Shared::Codec, Capture);
        IMPORTNAMESPACEANDTYPE(Shared::Codec, Data);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Codec, Index);
        IMPORTNAMESPACEANDTYPE(Shared::Codec, Play);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Codec, Status);

        IMPORTNAMESPACE(Shared::Codec, AuxiliaryInputControl);
        IMPORTNAMESPACE(Shared::Codec, DataFormat);
        IMPORTNAMESPACE(Shared::Codec, InputControl);
        IMPORTNAMESPACE(Shared::Codec, InterfaceControl);
        IMPORTNAMESPACE(Shared::Codec, LoopbackControl);
        IMPORTNAMESPACE(Shared::Codec, ModeAndId);
        IMPORTNAMESPACE(Shared::Codec, OutputControl);
        IMPORTNAMESPACE(Shared::Codec, PinControl);
        IMPORTNAMESPACE(Shared::Codec, PlaybackBaseCount);
        IMPORTNAMESPACE(Shared::Codec, ErrorStatusAndInitialization);

    }
}

// INTERFACE                           I/O,MEM,    R,W     ADDRESS
//                                     INT,DMA             HEX
// ------------------------------------------------------------
// UltraSound Base Port:               ---         --      2X0
// MIDI Interface:
//     Control                         I/O           W     3X0
//     Status                          I/O         R       3X0
//     Transmit Data                   I/O           W     3X1
//     Receive Data                    I/O         R       3X1
// Joystick Interface:
//     Trigger Timer                   I/O           W     201
//     Read Data                       I/O         R       201
// GF1 Synthesizer:
//     GF1 Page Register               I/O         R/W     3X2
//     GF1/Global Register Select      I/O         R/W     3X3
//     GF1/Global Data Low Byte        I/O         R/W     3X4
//     GF1/Global Data High Byte       I/O         R/W     3X5
//     IRQ Status Register 1=ACTIVE    I/O         R       2X6
//     Timer Control Reg               I/O         R/W     2X8
//     Timer Data                      I/O           W     2X9
//     DRAM                            I/O         R/W     3X7
//     DRAM                            DMA         R/W     1,3,5,6,7
//     Record Digital Audio            DMA         R       1,3,5,6,7
// BOARD ONLY
//     Mix Control register            I/O           W     2X0
//     IRQ control register            I/O           W     2XB
//     (2X0- bit 6 = 1)
//     DMA control register            I/O           W     2XB
//     (2X0- bit 6 = 0)
//     Register Controls               I/O         R/W     2XF (Rev 3.4+)
//     Board Version                   I/O         R       7X6 (Rev 3.7+)
// Mixer Control
//     Control Port                    I/O           W     7X6
//     Data Port                       I/O           W     3X6
// CODEC
//     Daughter Card                   I/O         R/W     530-533
//         or                          I/O         R/W     604-607
//         or                          I/O         R/W     E80-E83
//         or                          I/O         R/W     F40-F43
//     UltraMax                        I/O         R/W     3XC-3XF
//     Codec Addr Select               I/O         R/W     Base+0
//     Codec Data                      I/O         R/W     Base+1
//     Codec Status                    I/O         R/W     Base+2
//     Codec PIO                       I/O         R/W     Base+3

// These are the global registers. They are not voice-specific.
// Address     Mode    Width   Description
// 41          R/W      8      DRAM DMA Control
// 42            W     16      DMA Start Address
// 43            W     16      DRAM I/O Address (LOW)
// 44            W      8      DRAM I/O Address (HIGH)
// 45          R/W      8      Timer Control
// 46            W      8      Timer 1 Count
// 47            W      8      Timer 2 Count
// 48            W      8      Sampling Frequency
// 49          R/W      8      Sampling Control
// 4B            W      8      Joystick trim DAC
// 4C          R/W      8      RESET

// The following are the voice-specific registers. Each voice has its own bank of read and write registers that alter its
// behavior. The write registers range from 0 to F and the corresponding read registers range from 80 to 8F. To convert
// from the write to the read, just add 80 hex.

// Write   Read    Width   Description
// 0       80       8      Voice Control
// 1       81      16      Frequency Control
// 2       82      16      Starting Address (HIGH)
// 3       83      16      Starting Address (LOW)
// 4       84      16      End Address (HIGH)
// 5       85      16      End Address (LOW)
// 6       86       8      Volume Ramp Rate
// 7       87       8      Volume Ramp Start
// 8       88       8      Volume Ramp End
// 9       89      16      Current Volume
// A       8A      16      Current Address (HIGH)
// B       8B      16      Current Address (LOW)
// C       8C       8      Pan Position
// D       8D       8      Volume Control
// E       8E       8      Active Voices (Voice independent)
// F       8F       8      IRQ Status (Voice independent)

// There are several 'self-modifying' bits defined for these registers. This means that the GF1 may change them at
// anytime on its own. Due to the fact that the software must accommodate this phenomena, it is possible that the GF1
// may change something immediately after your application has set/reset one of the bits. This is due to the GF1's
// pipeline processor type of architecture: it does a read-modify-write cycle, and if your application modifies one of
// these bits AFTER it has done the read portion and BEFORE it does the write portion, it's possible for the chip to
// perform incorrectly. To overcome this, you need to do a double write (with a delay in between) when those particular
// bits are involved. This delay must be at least 3 times the length of time necessary to process a voice. (3*1.6
// microsecs). In the lowlevel code, this is done with a function called GF1_Delay. The self-modifying bits are
// designated with an (*) after the particular bit definition.