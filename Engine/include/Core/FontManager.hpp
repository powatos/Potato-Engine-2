/** @file "FontManager.hpp" */
#pragma once

#include <array>
#include <string_view>
#include <unordered_map>

#include "EngineSubsystem.hpp"
#include "Font.hpp"

constexpr int DEFAULT_FONT_PT_SIZE = 8;

class FontManager : public EngineSubsystem<FontManager>
{
public:
    FontManager();
    void Resolve() noexcept override;

    Font* GetFont(const std::string& relativePath, const int ptSize = DEFAULT_FONT_PT_SIZE);

    Font* CreateFont(const FilePath& rootPath, const int ptSize = DEFAULT_FONT_PT_SIZE);
    Font* ChangeFontSize(const Font* font, const int ptSize);

    static constexpr std::array<std::string_view, 1> validExtensions {
        ".ttf"
    };

private:
    std::unordered_map<std::string, std::unordered_map<int, Font*>> cache;

};
