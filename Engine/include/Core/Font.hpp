/** @file "Font.hpp" */
#pragma once
#include <unordered_map>

#include "Asset.hpp"

/**
 * @brief Font asset
 * @note Pt size is stored in the TTF_Font vector. This struct only represents the font styling; Specify font when getting the underlying pointer using Internal_GetFont()
 */
struct Font : public Asset
{
    Font(const FilePath& path, int size);
    Font(Font&& other) noexcept;

    ~Font();

    void* Internal_GetFont(int ptSize = -1);

private:
    int cachedDefaultSize;

    std::unordered_map<int, void*> ttf_fonts;

};
