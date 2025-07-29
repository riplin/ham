//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/ics/shared/data.h>

// Balance/Pan Mode
// The operation of the attenuator pair in balance/pan mode is
// controlled by the left control register. The data written to the
// left control register is also written to the right control register;
// therefore, need not write to the right control or right attenuation
// register while operating in this mode. The balance/pan register
// controls the gain of the attenuator pair by regulating the balance
// and the pan position of the output signals. The master attenu-
// ator pair does not have a mono switch and cannot be operated
// in any mode requiring the mono switch to be in the closed (or
// on) position. The master attenuator pair cannot be used in pan
// mode.

// In the pan/balance mode, two separate registers are used to
// control the attenuator pair. The attenuation value directed to
// the left attenuator register is modified by the contents of the
// pan/balance register, and the appropriate values are then writ-
// ten to the left and right attenuator registers. When the pan/bal-
// ance register of a channel is modified, the data value has no
// effect on the attenuator settings until the next value is written
// to the left attenuator register.

namespace Ham::IntegratedCircuitSystems::Shared
{

typedef Data_t BalancePan_t;
namespace BalancePan
{
    enum
    {
        Left = 0x00,
        Middle = 0x07,
        Right = 0x0F
    };
}

}