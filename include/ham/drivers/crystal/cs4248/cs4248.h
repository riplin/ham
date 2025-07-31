//Copyright 2025-Present riplin

#pragma once

#include <has/imports.h>

#include <ham/drivers/crystal/shared/data.h>
#include <ham/drivers/crystal/shared/index.h>
#include <ham/drivers/crystal/shared/status.h>
#include <ham/drivers/crystal/shared/play.h>
#include <ham/drivers/crystal/shared/capt.h>

#include <ham/drivers/crystal/shared/regs/auxic.h>
#include <ham/drivers/crystal/shared/regs/bc.h>
#include <ham/drivers/crystal/shared/regs/df.h>
#include <ham/drivers/crystal/shared/regs/ic.h>
#include <ham/drivers/crystal/shared/regs/itfconf.h>
#include <ham/drivers/crystal/shared/regs/lc.h>
#include <ham/drivers/crystal/shared/regs/misc.h>
#include <ham/drivers/crystal/shared/regs/oc.h>
#include <ham/drivers/crystal/shared/regs/pc.h>
#include <ham/drivers/crystal/shared/regs/tstinit.h>

#include <ham/drivers/crystal/shared/io/auxic.h>
#include <ham/drivers/crystal/shared/io/bc.h>
#include <ham/drivers/crystal/shared/io/df.h>
#include <ham/drivers/crystal/shared/io/ic.h>
#include <ham/drivers/crystal/shared/io/itfconf.h>
#include <ham/drivers/crystal/shared/io/lc.h>
#include <ham/drivers/crystal/shared/io/misc.h>
#include <ham/drivers/crystal/shared/io/oc.h>
#include <ham/drivers/crystal/shared/io/pc.h>
#include <ham/drivers/crystal/shared/io/tstinit.h>

namespace Ham::Crystal::CS4248
{
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, Index);
    IMPORTNAMESPACEANDTYPE(Shared, Data);
    IMPORTNAMESPACEANDTYPE(Shared, Status);
    IMPORTNAMESPACEANDTYPE(Shared, Capture);
    IMPORTNAMESPACEANDTYPE(Shared, Play);
    IMPORTNAMESPACE(Shared, Register);

    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, AuxiliaryInputControl);
    typedef Shared::BaseCount8_t BaseCount8_t;
    typedef Shared::BaseCount16_t BaseCount16_t;
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, DataFormat);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, InputControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, InterfaceControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, LoopbackControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, Miscellaneous);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, OutputControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, PinControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, TestAndInitialization);
    
    namespace IO
    {
        IMPORTNAMESPACE(Shared::IO, AuxiliaryInputControl);
        IMPORTNAMESPACE(Shared::IO, BaseCount);
        IMPORTNAMESPACE(Shared::IO, DataFormat);
        IMPORTNAMESPACE(Shared::IO, InputControl);
        IMPORTNAMESPACE(Shared::IO, InterfaceControl);
        IMPORTNAMESPACE(Shared::IO, LoopbackControl);
        IMPORTNAMESPACE(Shared::IO, Miscellaneous);
        IMPORTNAMESPACE(Shared::IO, OutputControl);
        IMPORTNAMESPACE(Shared::IO, PinControl);
        IMPORTNAMESPACE(Shared::IO, TestAndInitialization);
    }
}

// Index   Register Name
// 0       Left Input Control
// 1       Right Input Control
// 2       Left Aux #1 Input Control
// 3       Right Aux #1 Input Control
// 4       Left Aux #2 Input Control.
// 5       Right Aux #2 Input Control
// 6       Left Output Control
// 7       Right Output Control
// 8       Data Format
// 9       Interface Configuration
// 10      Pin Control
// 11      Test and Initialization
// 12      Misc. Information
// 13      Loopback Control
// 14      Upper Base Count
// 15      Lower Base Count
