#include <stdio.h>

#include <support/allocatr.h>
#include <ham/drivers/gravis/shared/system.h>

int
main(void)
{
    using namespace Ham::Gravis::Shared;

    Support::Allocator allocator;

    auto result = Function::System::Initialize(allocator);

    if (result != Function::System::InitializeError::Success)
    {
        printf("Error: %s\n", Function::System::InitializeError::ToString(result));
        return -1;
    }

    //

    Function::System::Shutdown();

    return 0;
}
