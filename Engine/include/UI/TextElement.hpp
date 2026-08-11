/** @file TextElement.hpp */
#pragma once

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

    void SetFont(Font* font); ///< @brief Set text font
    Font* GetFont() const; ///< @brief Gets current text font

    void SetColor(Color fontColor); ///< @brief Set text color
    Color GetColor() const; ///< @brief Gets current text color

    void SetPtSize(int size); ///< @brief Set font size
    int GetPtSize() const; ///< @brief Gets current font size

    void* ___Get_Text() const;

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


