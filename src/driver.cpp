//Copyright 2025-Present riplin

#include <ham/driver.h>

namespace Ham::Driver
{

void Base::Destroy(Base* base)
{
    if (base != nullptr)
    {
        Has::IAllocator& allocator = base->m_Allocator;
        base->~Base();
        allocator.Free(base);
    }
}

}