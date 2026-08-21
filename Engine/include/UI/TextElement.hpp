/** @file TextElement.hpp */
#pragma once

#include <nlohmann/detail/macro_scope.hpp>

#include "WidgetElement.hpp"
#include "Core/Font.hpp"

/**
 * @brief Enum representing the modes of displaying text on a TextElement
 */
enum class TextMode
{
    IgnoreBounds, ///< @brief Ignore bounding box and render all text normally
    ResizeToFit, ///< @brief Resize the bounding box to fit text
    ShrinkToFit ///< @brief Shrink the text to fit bounding box
};

/**
 * @brief Element that displays text on a widget
 */
struct TextElement : public WidgetElement
{
    DEFINE_RENDER_PASSTHROUGH

    TextElement(const std::string& UID);
    virtual ~TextElement();

    void SetText(const std::string& text); ///< @brief Set text to display
    std::string GetText() const; ///< @brief Get text currently displayed

    void SetWrapText(bool wrapText); ///< @brief Set text wrapping enabled
    bool IsWrappingText() const; ///< @brief Get text wrapping enabled

    void SetTextMode(TextMode textMode); ///< @brief Set text bounding box mode
    TextMode GetTextMode() const; ///< @brief Get text bounding box mode

    // TODO: decide how to set in presets
    void SetFont(Font* font); ///< @brief Set text ttf_font
    Font* GetFont() const; ///< @brief Gets current text ttf_font

    void SetColor(Color fontColor); ///< @brief Set text color
    Color GetColor() const; ///< @brief Gets current text color

    void SetPtSize(int size); ///< @brief Set ttf_font size
    int GetPtSize() const; ///< @brief Gets current ttf_font size

    void* Internal_GetText() const;

    virtual void SetScreenSize(const UIVector& size) override;

private:
    void resizeBox();
    void shrinkText();

    std::string cachedText;
    Color cachedColor;
    int ptSize;

    Font* textFont;

    void* ttf_text;

    bool WrapText;
    TextMode Mode;


};

NLOHMANN_JSON_SERIALIZE_ENUM(TextMode, {
    { TextMode::IgnoreBounds, "IgnoreBounds" },
    { TextMode::ResizeToFit, "ResizeToFit" },
    { TextMode::ShrinkToFit, "ShrinkToFit" }
})

BOOST_DESCRIBE_STRUCT(TextElement,
    (WidgetElement),
    (
        SetText,
        SetWrapText,
        SetTextMode,
        // SetFont,
        SetColor,
        SetPtSize
    )
)
