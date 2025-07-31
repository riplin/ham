//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/ics/ics2101/ics2101.h>
#include <ham/drivers/gravis/shared/regtype.h>

namespace Ham::Gravis::Shared::Mixer
{

namespace Register
{
    enum
    {
        Address = 0x506
    };
}

typedef IntegratedCircuitSystems::ICS2101::Address_t Address_t;
namespace Address
{

    enum
    {
        ControlLeft = IntegratedCircuitSystems::ICS2101::Address::ControlLeft,
        ControlRight = IntegratedCircuitSystems::ICS2101::Address::ControlRight,
        AttenuatorLeft = IntegratedCircuitSystems::ICS2101::Address::AttenuatorLeft,
        AttenuatorRight = IntegratedCircuitSystems::ICS2101::Address::AttenuatorRight,
        BalancePan = IntegratedCircuitSystems::ICS2101::Address::BalancePan,

        Channel = IntegratedCircuitSystems::ICS2101::Address::Channel,
        MicrophoneIn = IntegratedCircuitSystems::ICS2101::Address::Pair1,
        LineIn = IntegratedCircuitSystems::ICS2101::Address::Pair2,
        CompactDiscIn = IntegratedCircuitSystems::ICS2101::Address::Pair3,
        GF1Out = IntegratedCircuitSystems::ICS2101::Address::Pair4,
        MasterOut = IntegratedCircuitSystems::ICS2101::Address::Master
    };

    inline void Write(Register_t baseAddress, Address_t channel, Address_t value)
    {
        IntegratedCircuitSystems::ICS2101::Address::Write(baseAddress + Register::Address, channel, value);
    }

}

}
