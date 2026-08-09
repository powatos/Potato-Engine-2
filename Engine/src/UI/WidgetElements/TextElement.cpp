/** @file TextElement.cpp */

#include "UI/TextElement.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include "Core/FontManager.hpp"
#include "Core/IWindowController.hpp"
#include "Core/PotatoEngine.hpp"
#include "Debug/Log.hpp"

TextElement::TextElement(const std::string& UID) : WidgetElement(UID) {

    textFont = FontManager::Get()->GetFont("");

    TTF_Font* ttf_font = static_cast<TTF_Font*>(textFont->___Get_Font());
    TTF_TextEngine* textEngine = static_cast<TTF_TextEngine*>(PotatoEngine::Get().GetWindowController()->RequestTTFEngine());

    TTF_Text* text = TTF_CreateText(
        textEngine,
        ttf_font,
        "TextElement",
        0
    );

    ttf_text = text;

    TTF_SetTextColor(text, 0x00, 0x00, 0x00, 0xff);

    Mode = TextMode::IgnoreBounds;
    WrapText = false;

}

void TextElement::SetText(const std::string& text) {

    TTF_SetTextString(static_cast<TTF_Text*>(ttf_text), text.c_str(), 0);

    if (text.length() != cachedText.length()) {
        cachedText = text;

        if (Mode == TextMode::ShrinkToFit) {
            shrinkText();
        } else if (Mode == TextMode::ResizeToFit) {
            resizeBox();
        }

        return;
    }

    cachedText = text;
}
std::string TextElement::GetText() const {
    return cachedText;
}

void TextElement::SetWrapText(bool wrapText) {

    TTF_Text* text = static_cast<TTF_Text*>(ttf_text);

    int h;
    TTF_GetTextSize(text, nullptr, &h);

    if (!wrapText || h <= GetTrueSize().y) {
        TTF_SetTextWrapWidth(text, 0);
        return;
    }

    TTF_SetTextWrapWidth(text, GetTrueSize().x);
}
bool TextElement::IsWrappingText() const {
    return WrapText;
}

void TextElement::SetTextMode(TextMode textMode) {
    Mode = textMode;

    if (Mode == TextMode::ResizeToFit) {
        resizeBox();
    } else if (Mode == TextMode::ShrinkToFit) {
        shrinkText();
    }

}
TextMode TextElement::GetTextMode() const {
    return Mode;
}

void TextElement::SetFont(Font* font) {
    if (textFont == nullptr) {
        LOG(LogType::WARNING, "Attempt to set invalid font to TextElement");
        return;
    }

    textFont = font;

    TTF_Text* text = static_cast<TTF_Text*>(ttf_text);
    TTF_SetTextFont(text, static_cast<TTF_Font*>(font->___Get_Font()));

    if (Mode == TextMode::ShrinkToFit) {
        shrinkText();
    }

}
Font* TextElement::GetFont() const {
    return textFont;
}

void TextElement::SetColor(Color fontColor) {

    TTF_SetTextColor(static_cast<TTF_Text*>(ttf_text), fontColor.R, fontColor.G, fontColor.B, fontColor.A);

    cachedColor = fontColor;
}
Color TextElement::GetColor() const {
    return cachedColor;
}

void TextElement::SetPtSize(int size) {

    if (Mode == TextMode::ShrinkToFit) {
        return;
    }

    if (size < 1) { size = 1; }

    textFont = FontManager::Get()->ChangeFontSize(textFont, size);
    TTF_SetTextFont(static_cast<TTF_Text*>(ttf_text), static_cast<TTF_Font*>(textFont->___Get_Font()));

    if (Mode == TextMode::ResizeToFit) {
        resizeBox();
    }

}
int TextElement::GetPtSize() const {
    return ptSize;
}

void TextElement::SetScreenSize(const UIVector& size) {
    if (Mode == TextMode::ResizeToFit) { return; }

    WidgetElement::SetScreenSize(size);

    if (Mode == TextMode::ShrinkToFit) {
        shrinkText();
        return;
    }

    SetWrapText(WrapText);

}

void TextElement::resizeBox() {
    TTF_Text* text = static_cast<TTF_Text*>(ttf_text);

    int w,h;
    TTF_GetTextSize(text, &w, &h);

    const Vector2 trueSize = GetTrueSize();

    WidgetElement::SetScreenSize(
        GetScreenSize() + UIVector(0.f, 0.f, w - trueSize.x, h - trueSize.y)
    );

}


void TextElement::shrinkText() {
    // binary search for fitting ptsize

    const Vector2 trueSize = GetTrueSize();

    // make a temporary copy of the font to perform logic
    TTF_Text* text = static_cast<TTF_Text*>(ttf_text);
    TTF_Font* probeFont = TTF_CopyFont(static_cast<TTF_Font*>(textFont->___Get_Font()));

    TTF_SetTextFont(text, probeFont);

    auto checkFits = [&](int size) -> bool {
        TTF_SetFontSize(probeFont, static_cast<float>(size));

        int w,h;
        TTF_GetTextSize(text, &w, &h);

        return w <= trueSize.x && h <= trueSize.y;
    };

    int result;

    if (!checkFits(1)) {
        result = 1;
    } else {
        int low = 1, high = 1;
        while (checkFits(high)) {
            low = high;
            high *= 2;
        }

        while (high - low > 1) {
            const int mid = low + (high - low) / 2;
            const bool b = checkFits(mid);
            high = b ? high : low;
            low = b ? mid : low;
        }

        result = low;
    }

    textFont = FontManager::Get()->ChangeFontSize(textFont, result);

    TTF_SetTextFont(text, static_cast<TTF_Font*>(textFont->___Get_Font()));
    TTF_CloseFont(probeFont);

}

void* TextElement::___Get_Text() const {
    return ttf_text;
}

TextElement::~TextElement() {
    TTF_DestroyText(static_cast<TTF_Text*>(ttf_text));
}
