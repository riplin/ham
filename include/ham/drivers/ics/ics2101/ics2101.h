//Copyright 2025-Present riplin

#pragma once

#include <has/imports.h>

#include <ham/drivers/ics/shared/data.h>
#include <ham/drivers/ics/shared/address.h>
#include <ham/drivers/ics/shared/regs/attn.h>
#include <ham/drivers/ics/shared/regs/balpan.h>
#include <ham/drivers/ics/shared/regs/ctrll.h>
#include <ham/drivers/ics/shared/regs/ctrlr.h>
#include <ham/drivers/ics/shared/regs/attn.h>
#include <ham/drivers/ics/shared/regs/balpan.h>
#include <ham/drivers/ics/shared/regs/ctrll.h>
#include <ham/drivers/ics/shared/regs/ctrlr.h>
#include <ham/drivers/ics/shared/io/attn.h>
#include <ham/drivers/ics/shared/io/balpan.h>
#include <ham/drivers/ics/shared/io/ctrll.h>
#include <ham/drivers/ics/shared/io/ctrlr.h>

namespace Ham::IntegratedCircuitSystems::ICS2101
{
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, Address);
    IMPORTNAMESPACEANDTYPE(Shared, Data);
    IMPORTNAMESPACEANDTYPE(Shared, Attenuation);
    IMPORTNAMESPACEANDTYPE(Shared, BalancePan);
    IMPORTNAMESPACEANDTYPE(Shared, ControlLeft);
    IMPORTNAMESPACEANDTYPE(Shared, ControlRight);

    IMPORTNAMESPACE(Shared, Attenuation);
    IMPORTNAMESPACE(Shared, BalancePan);
    IMPORTNAMESPACE(Shared, ControlLeft);
    IMPORTNAMESPACE(Shared, ControlRight);

    namespace IO
    {
        IMPORTNAMESPACE(Shared::IO, Attenuation);
        IMPORTNAMESPACE(Shared::IO, BalancePan);
        IMPORTNAMESPACE(Shared::IO, ControlLeft);
        IMPORTNAMESPACE(Shared::IO, ControlRight);
    }
}