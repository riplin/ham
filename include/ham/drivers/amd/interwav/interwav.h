//Copyright 2025-Present riplin

#pragma once

#include <has/imports.h>
#include <ham/drivers/amd/shared/system.h>

#include <has/system/pnp/pnp.h>
#include <ham/drivers/amd/shared/pnp/id.h>
#include <ham/drivers/amd/shared/pnp/device.h>
#include <ham/drivers/amd/shared/pnp/control/dmasel.h>
#include <ham/drivers/amd/shared/pnp/control/irqsel.h>
#include <ham/drivers/amd/shared/pnp/control/pwrmod.h>
#include <ham/drivers/amd/shared/pnp/control/irqtype.h>
#include <ham/drivers/amd/shared/pnp/control/softrst.h>
#include <ham/drivers/amd/shared/pnp/control/addrctrl.h>
#include <ham/drivers/amd/shared/pnp/control/eepromct.h>
#include <ham/drivers/amd/shared/pnp/control/eepromen.h>

#include <ham/drivers/crystal/cs4231/cs4231.h>

#include <ham/drivers/amd/shared/codec/cdf.h>
#include <ham/drivers/amd/shared/codec/cfg2.h>
#include <ham/drivers/amd/shared/codec/cfg3.h>
#include <ham/drivers/amd/shared/codec/mioc.h>
#include <ham/drivers/amd/shared/codec/synin.h>
#include <ham/drivers/amd/shared/codec/aux2in.h>
#include <ham/drivers/amd/shared/codec/extctl.h>
#include <ham/drivers/amd/shared/codec/outattn.h>
#include <ham/drivers/amd/shared/codec/modselid.h>
#include <ham/drivers/amd/shared/codec/micinctl.h>
#include <ham/drivers/amd/shared/codec/plvarfrq.h>

#include <ham/drivers/gravis/max/max.h>

namespace Ham::Amd::InterWave
{
    using namespace Shared::System;
    IMPORTNAMESPACEANDTYPE(Shared::System, InitializeError);
    namespace PnP
    {
        IMPORTNAMESPACEANDTYPE(Has::System::PnP, Config);
        IMPORTNAMESPACEANDTYPE(Has::System::PnP, CardSelectNumber);
        IMPORTNAMESPACEANDTYPE(Has::System::PnP, LogicalDevice);
        IMPORTNAMESPACEANDTYPE(Has::System::PnP, VendorId);
        IMPORTNAMESPACE(Has::System::PnP, ResourceData);
        IMPORTNAMESPACEANDTYPE(Has::System::PnP, ReadDataPort);
        IMPORTNAMESPACEANDTYPE(Has::System::PnP, SerialIsolation);
        IMPORTNAMESPACEANDTYPE(Has::System::PnP, Status);
        IMPORTNAMESPACE(Has::System::PnP, WakeCsn);
        IMPORTNAMESPACEANDTYPE(Has::System::PnP, Activate);
        IMPORTNAMESPACEANDTYPE(Has::System::PnP, RangeCheck);

        IMPORTNAMESPACEANDTYPE(Shared::PnP, Identifier);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, Device);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, AddressControl);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, DmaSelect);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, EepromControl);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, EepromEnable);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, InterruptSelect);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, InterruptType);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, PowerMode);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, SoftReset);
    }

    namespace Codec
    {
        IMPORTNAMESPACEANDTYPEANDSHIFT(Crystal::CS4231, Index);
        IMPORTNAMESPACEANDTYPE(Crystal::CS4231, Data);
        IMPORTNAMESPACEANDTYPE(Crystal::CS4231, Status);
        IMPORTNAMESPACEANDTYPE(Crystal::CS4231, Capture);
        IMPORTNAMESPACEANDTYPE(Crystal::CS4231, Play);
        IMPORTNAMESPACE(Crystal::CS4231, Register);

        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Codec, SynthesizerInputControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Codec, Auxiliary2InputControl);
        IMPORTNAMESPACEANDTYPEANDSHIFTAS(Crystal::CS4231, DataFormat, PlaybackDataFormat);
        IMPORTNAMESPACEANDTYPEANDSHIFTAS(Crystal::CS4231, InputControl, Configuration1);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Crystal::CS4231, InterfaceControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Crystal::CS4231, LoopbackControl);
        IMPORTNAMESPACEANDTYPE(Shared::Codec, ModeSelectId);
        IMPORTNAMESPACEANDTYPEANDSHIFTAS(Crystal::CS4231, OutputControl, PlaybackDacControl);
        typedef Crystal::CS4231::PlaybackBaseCount8_t PlaybackBaseCount8_t;
        typedef Crystal::CS4231::PlaybackBaseCount16_t PlaybackBaseCount16_t;
        IMPORTNAMESPACEANDTYPE(Shared::Codec, ExternalControl);
        IMPORTNAMESPACEANDTYPEANDSHIFTAS(Crystal::CS4231, ErrorStatusAndInitialization, CodecStatus2);
        IMPORTNAMESPACEANDTYPE(Shared::Codec, Configuration2);
        IMPORTNAMESPACEANDTYPE(Shared::Codec, Configuration3);
        typedef Crystal::CS4231::CaptureBaseCount8_t CaptureBaseCount8_t;
        typedef Crystal::CS4231::CaptureBaseCount16_t CaptureBaseCount16_t;
        IMPORTNAMESPACEANDTYPE(Shared::Codec, CaptureDataFormat);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Crystal::CS4231, LineInputControl);
        typedef Crystal::CS4231::Timer8_t Timer8_t;
        typedef Crystal::CS4231::Timer16_t Timer16_t;
        IMPORTNAMESPACEANDTYPE(Shared::Codec, MicrophoneInputControl);
        IMPORTNAMESPACEANDTYPEANDSHIFTAS(Crystal::CS4231, AlternateFeatureStatus, CodecStatus3);
        IMPORTNAMESPACEANDTYPE(Shared::Codec, OutputAttenuation);
        IMPORTNAMESPACEANDTYPE(Shared::Codec, MonoInputOutputControl);
        typedef Shared::Codec::PlaybackVariableFrequency_t PlaybackVariableFrequency_t;

        namespace IO
        {
            IMPORTNAMESPACE(Shared::Codec::IO, SynthesizerInputControl);
            IMPORTNAMESPACE(Shared::Codec::IO, Auxiliary2InputControl);
            IMPORTNAMESPACEAS(Crystal::CS4231::IO, DataFormat, PlaybackDataFormat);
            IMPORTNAMESPACE(Crystal::CS4231::IO, InputControl);
            IMPORTNAMESPACEAS(Crystal::CS4231::IO, InterfaceControl, Configuration1);
            IMPORTNAMESPACE(Crystal::CS4231::IO, LoopbackControl);
            IMPORTNAMESPACE(Shared::Codec::IO, ModeSelectId);
            IMPORTNAMESPACEAS(Crystal::CS4231::IO, OutputControl, PlaybackDacControl);
            IMPORTNAMESPACE(Shared::Codec::IO, ExternalControl);
            IMPORTNAMESPACE(Crystal::CS4231::IO, PlaybackBaseCount);
            IMPORTNAMESPACEAS(Crystal::CS4231::IO, ErrorStatusAndInitialization, CodecStatus2);
            IMPORTNAMESPACE(Shared::Codec::IO, Configuration2);
            IMPORTNAMESPACE(Shared::Codec::IO, Configuration3);
            IMPORTNAMESPACE(Crystal::CS4231::IO, CaptureBaseCount);
            IMPORTNAMESPACE(Shared::Codec::IO, CaptureDataFormat);
            IMPORTNAMESPACE(Crystal::CS4231::IO, LineInputControl);
            IMPORTNAMESPACE(Crystal::CS4231::IO, Timer);
            IMPORTNAMESPACE(Shared::Codec, MicrophoneInputControl);
            IMPORTNAMESPACEAS(Crystal::CS4231::IO, AlternateFeatureStatus, CodecStatus3);
            IMPORTNAMESPACE(Shared::Codec::IO, OutputAttenuation);
            IMPORTNAMESPACE(Shared::Codec::IO, MonoInputOutputControl);
            IMPORTNAMESPACE(Shared::Codec::IO, PlaybackVariableFrequency);
        }
    }

    namespace Synthesizer
    {

    }
}