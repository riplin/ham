//Copyright 2025-Present riplin

#include <stdio.h>
#include <sys/nearptr.h>
#include <has/testing/log.h>
#include <support/allocatr.h>

#include <ham/drivers/amd/shared/system.h>

int main(int argc, const char** argv)
{
    __djgpp_nearptr_enable();

    LOG_CONFIGURE("test.txt");

    Support::Allocator allocator;

    Ham::Amd::Shared::System::Initialize(allocator);


    Ham::Amd::Shared::System::Shutdown();

    return 0;
}
