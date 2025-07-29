//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/ics/shared/data.h>

// Attenuation Register
// Each attenuator is controlled by a 7-bit value written to the
// attenuation register. The values of 127 through 16 will increase the
// attenuation linearly in one-half decibel (dB) increments. Val-
// ues of 15 through 0 will cause the attenuation to increase at an
// increasing rate, with a value of 00H corresponding to maxi-
// mum attenuation. The channel is off when the control register
// value is 00H and at maximum volume (completely on) with a
// value of 7FH.

namespace Ham::IntegratedCircuitSystems::Shared
{

typedef Data_t Attenuation_t;
namespace Attenuation
{
    enum
    {
        Mute = 0x00,
        MaxVolume = 0x7F
    };
}

}