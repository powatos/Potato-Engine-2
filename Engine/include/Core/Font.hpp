/** @file "Font.hpp" */
#pragma once
#include <unordered_map>

#include "Asset.hpp"

struct Font : public Asset
{
    Font(FilePath path, void* font);
    Font(Font&& other) noexcept;

    ~Font();

    void* Internal_GetFont();

private:

    void* font;

};
