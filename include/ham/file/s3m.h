//Copyright 2025-Present riplin

#pragma once

#include <has/ialloc.h>

namespace Ham::Music { class Song; }

namespace Ham::File::S3m
{

    extern Music::Song* Load(Has::IAllocator& allocator, const char* filePath);

}