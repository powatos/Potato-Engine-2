/** @file Asset.cpp */

#include "Core/Asset.hpp"

Asset::Asset(const FilePath& path) :
    meta({
        // .type = parseExtension(path.extension().string())
    }),
    path(path)

{}