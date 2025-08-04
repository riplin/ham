//Copyright 2025-Present riplin

#pragma once

#include <has/imports.h>
#include <has/system/pnp/pnp.h>
#include <ham/drivers/amd/shared/system.h>
#include <ham/drivers/amd/shared/pnp/device.h>
#include <ham/drivers/amd/shared/pnp/control/dmasel.h>
#include <ham/drivers/amd/shared/pnp/control/irqsel.h>
#include <ham/drivers/amd/shared/pnp/control/pwrmod.h>
#include <ham/drivers/amd/shared/pnp/control/rngchk.h>
#include <ham/drivers/amd/shared/pnp/control/irqtype.h>
#include <ham/drivers/amd/shared/pnp/control/softrst.h>
#include <ham/drivers/amd/shared/pnp/control/activate.h>
#include <ham/drivers/amd/shared/pnp/control/addrctrl.h>
#include <ham/drivers/amd/shared/pnp/control/eepromct.h>
#include <ham/drivers/amd/shared/pnp/control/eepromen.h>

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

        IMPORTNAMESPACEANDTYPE(Shared::PnP, Device);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, Activate);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, AddressControl);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, DmaSelect);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, EepromControl);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, EepromEnable);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, InterruptSelect);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, InterruptType);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, PowerMode);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, RangeCheck);
        IMPORTNAMESPACEANDTYPE(Shared::PnP, SoftReset);
    }

}