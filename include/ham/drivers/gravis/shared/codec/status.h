//Copyright 2025-Present riplin

#pragma once

#include <ham/drivers/crystal/cs4231/cs4231.h>
#include <ham/drivers/gravis/shared/regtype.h>
#include <ham/drivers/gravis/shared/codec/base.h>

namespace Ham::Gravis::Shared::Codec
{

typedef Crystal::CS4231::Status_t Status_t;
namespace Status
{

    enum
    {
        InterruptStatus = Crystal::CS4231::Status::InterruptStatus,
        InterruptClear = Crystal::CS4231::Status::InterruptClear,
        InterruptPending = Crystal::CS4231::Status::InterruptPending,
        PlaybackDataStatus = Crystal::CS4231::Status::PlaybackDataStatus,
        PlaybackDataReady = Crystal::CS4231::Status::PlaybackDataReady,
        PlaybackDataStale = Crystal::CS4231::Status::PlaybackDataStale,
        PlaybackChannel = Crystal::CS4231::Status::PlaybackChannel,
        PlaybackRight = Crystal::CS4231::Status::PlaybackRight,
        PlaybackLeft = Crystal::CS4231::Status::PlaybackLeft,
        PlaybackByte = Crystal::CS4231::Status::PlaybackByte,
        PlaybackLower = Crystal::CS4231::Status::PlaybackLower,
        PlaybackUpper = Crystal::CS4231::Status::PlaybackUpper,
        SampleStatus = Crystal::CS4231::Status::SampleStatus,
        SampleOk = Crystal::CS4231::Status::SampleOk,
        SampleError = Crystal::CS4231::Status::SampleError,
        CaptureStatus = Crystal::CS4231::Status::CaptureStatus,
        CaptureStale = Crystal::CS4231::Status::CaptureStale,
        CaptureFresh = Crystal::CS4231::Status::CaptureFresh,
        CaptureChannel = Crystal::CS4231::Status::CaptureChannel,
        CaptureRight = Crystal::CS4231::Status::CaptureRight,
        CaptureLeft = Crystal::CS4231::Status::CaptureLeft,
        CaptureByte = Crystal::CS4231::Status::CaptureByte,
        CaptureLower = Crystal::CS4231::Status::CaptureLower,
        CaptureUpper = Crystal::CS4231::Status::CaptureUpper
    };
    
    namespace Shift
    {
        using namespace Crystal::CS4231::Status::Shift;
    }

    inline Status_t Read(Register_t baseAddress)
    {
        return Crystal::CS4231::Status::Read(baseAddress + Register::Base);
    }

    inline void Write(Register_t baseAddress, Status_t value)
    {
        Crystal::CS4231::Status::Write(baseAddress + Register::Base, value);
    }
    
}

}