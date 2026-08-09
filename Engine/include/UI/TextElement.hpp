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
    ~TextElement();

    void SetText(const std::string& text);
    std::string GetText() const;

    void SetWrapText(bool wrapText);
    bool IsWrappingText() const;

    void SetTextMode(TextMode textMode);
    TextMode GetTextMode() const;

    void SetFont(Font* font);
    Font* GetFont() const;

    void SetColor(Color fontColor);
    Color GetColor() const;

    void SetPtSize(int size);
    int GetPtSize() const;

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


