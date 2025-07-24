//Copyright 2025-Present riplin

#pragma once

#include <has/ialloc.h>

namespace Ham::File { class Song; }

namespace Ham::File::Mod
{

    extern Song* Load(Has::IAllocator& allocator, const char* filePath);

}