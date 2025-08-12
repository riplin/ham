//Copyright 2025-Present riplin

#pragma once

#include <has/imports.h>

#include <ham/drivers/crystal/shared/index2.h>
#include <ham/drivers/crystal/shared/data.h>
#include <ham/drivers/crystal/shared/status.h>
#include <ham/drivers/crystal/shared/play.h>
#include <ham/drivers/crystal/shared/capt.h>

#include <ham/drivers/crystal/shared/regs/auxic.h>
#include <ham/drivers/crystal/shared/regs/df2.h>
#include <ham/drivers/crystal/shared/regs/ic.h>
#include <ham/drivers/crystal/shared/regs/itfconf.h>
#include <ham/drivers/crystal/shared/regs/lc.h>
#include <ham/drivers/crystal/shared/regs/modeid.h>
#include <ham/drivers/crystal/shared/regs/oc.h>
#include <ham/drivers/crystal/shared/regs/pbc.h>
#include <ham/drivers/crystal/shared/regs/pc.h>
#include <ham/drivers/crystal/shared/regs/tstinit.h>

#include <ham/drivers/crystal/shared/regs/afei.h>
#include <ham/drivers/crystal/shared/regs/afeii.h>
#include <ham/drivers/crystal/shared/regs/afs.h>
#include <ham/drivers/crystal/shared/regs/cbc.h>
#include <ham/drivers/crystal/shared/regs/cdf.h>
#include <ham/drivers/crystal/shared/regs/lic.h>
#include <ham/drivers/crystal/shared/regs/mioc.h>
#include <ham/drivers/crystal/shared/regs/timer.h>
#include <ham/drivers/crystal/shared/regs/vcid.h>

#include <ham/drivers/crystal/shared/io/auxic.h>
#include <ham/drivers/crystal/shared/io/df2.h>
#include <ham/drivers/crystal/shared/io/ic.h>
#include <ham/drivers/crystal/shared/io/itfconf.h>
#include <ham/drivers/crystal/shared/io/lc.h>
#include <ham/drivers/crystal/shared/io/modeid.h>
#include <ham/drivers/crystal/shared/io/oc.h>
#include <ham/drivers/crystal/shared/io/pbc.h>
#include <ham/drivers/crystal/shared/io/pc.h>
#include <ham/drivers/crystal/shared/io/tstinit.h>

#include <ham/drivers/crystal/shared/io/afei.h>
#include <ham/drivers/crystal/shared/io/afeii.h>
#include <ham/drivers/crystal/shared/io/afs.h>
#include <ham/drivers/crystal/shared/io/cbc.h>
#include <ham/drivers/crystal/shared/io/cdf.h>
#include <ham/drivers/crystal/shared/io/lic.h>
#include <ham/drivers/crystal/shared/io/mioc.h>
#include <ham/drivers/crystal/shared/io/timer.h>
#include <ham/drivers/crystal/shared/io/vcid.h>

namespace Ham::Crystal::CS4231
{

    IMPORTNAMESPACEANDTYPEANDSHIFTAS(Shared, Index2, Index);
    IMPORTNAMESPACEANDTYPE(Shared, Data);
    IMPORTNAMESPACEANDTYPE(Shared, Status);
    IMPORTNAMESPACEANDTYPE(Shared, Capture);
    IMPORTNAMESPACEANDTYPE(Shared, Play);
    IMPORTNAMESPACE(Shared, Register);

    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, AuxiliaryInputControl);
    IMPORTNAMESPACEANDTYPEANDSHIFTAS(Shared, DataFormat2, DataFormat);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, InputControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, InterfaceControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, LoopbackControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, ModeAndId);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, OutputControl);
    typedef Shared::PlaybackBaseCount8_t PlaybackBaseCount8_t;
    typedef Shared::PlaybackBaseCount16_t PlaybackBaseCount16_t;
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, PinControl);
    IMPORTNAMESPACEANDTYPEANDSHIFTAS(Shared, TestAndInitialization, ErrorStatusAndInitialization);

    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, AlternateFeatureEnableI);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, AlternateFeatureEnableII);
    typedef Shared::CaptureBaseCount8_t CaptureBaseCount8_t;
    typedef Shared::CaptureBaseCount16_t CaptureBaseCount16_t;
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, CaptureDataFormat);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, LineInputControl);
    typedef Shared::Timer8_t Timer8_t;
    typedef Shared::Timer16_t Timer16_t;
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, AlternateFeatureStatus);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MonoInputOutputControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, VersionAndChipId);

    namespace IO
    {
        IMPORTNAMESPACE(Shared::IO, AuxiliaryInputControl);
        IMPORTNAMESPACEAS(Shared::IO, DataFormat2, DataFormat);
        IMPORTNAMESPACE(Shared::IO, InputControl);
        IMPORTNAMESPACE(Shared::IO, InterfaceControl);
        IMPORTNAMESPACE(Shared::IO, LoopbackControl);
        IMPORTNAMESPACE(Shared::IO, ModeAndId);
        IMPORTNAMESPACE(Shared::IO, OutputControl);
        IMPORTNAMESPACE(Shared::IO, PinControl);
        IMPORTNAMESPACE(Shared::IO, PlaybackBaseCount);
        IMPORTNAMESPACEAS(Shared::IO, TestAndInitialization, ErrorStatusAndInitialization);
        IMPORTNAMESPACE(Shared::IO, AlternateFeatureEnableI);
        IMPORTNAMESPACE(Shared::IO, AlternateFeatureEnableII);
        IMPORTNAMESPACE(Shared::IO, CaptureBaseCount);
        IMPORTNAMESPACE(Shared::IO, CaptureDataFormat);
        IMPORTNAMESPACE(Shared::IO, LineInputControl);
        IMPORTNAMESPACE(Shared::IO, Timer);
        IMPORTNAMESPACE(Shared::IO, AlternateFeatureStatus);
        IMPORTNAMESPACE(Shared::IO, MonoInputOutputControl);
        IMPORTNAMESPACE(Shared::IO, VersionAndChipId);
    }
}

// Index   Register Name
//  I0     Left ADC Input Control
//  I1     Right ADC Input Control
//  I2     Left Aux #1 Input Control
//  I3     Right Aux #1 Input Control
//  I4     Left Aux #2 Input Control
//  I5     Right Aux #2 Input Control
//  I6     Left DAC Output Control
//  I7     Right DAC Output Control
//  I8     Fs & Playback Data Format
//  I9     Interface Configuration
// I10     Pin Control
// I11     Error Status and Initialization
// I12     MODE and ID (MODE2 bit)
// I13     Loopback Control
// I14     Playback Upper Base Count
// I15     Playback Lower Base Count
// I16     Alternate Feature Enable I
// I17     Alternate Feature Enable II
// I18     Left Line Input Control
// I19     Right Line Input Control
// I20     Timer Low Byte
// I21     Timer High Byte
// I22     RESERVED
// I23     RESERVED
// I24     Alternate Feature Status
// I25     Version / Chip ID
// I26     Mono Input & Output Control
// I27     RESERVED
// I28     Capture Data Format
// I29     RESERVED
// I30     Capture Upper Base Count
// I31     Capture Lower Base Count