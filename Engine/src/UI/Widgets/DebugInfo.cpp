/** @file DebugInfo.cpp */

#include "Core/GameInstance.hpp"
#include "UI/TextElement.hpp"

#include "Widgets/DebugInfo.hpp"

#include "UI/BoxElement.hpp"
#include "UI/ResizeElement.hpp"
#include "UI/UIManager.hpp"

void DebugInfo::Setup() {
    Widget::Setup();

    UIManager* uim = UIManager::Get();

    SetScreenPosition(UIVector(0, 0, 10, 10));
    SetScreenSize(UIVector(0.1, 0.3, 0, 0));

    BoxElement* BG = uim->AddUI<BoxElement>("BG", this);
    BG->SetScreenSize(UIVector(1.f, 1.f, 0.f, 0.f));
    BG->BorderWeight = 1;
    BG->BorderColor = Color::BLACK();

    ResizeElement* resizer = uim->AddUI<ResizeElement>("ResizeElement", this);
    resizer->ResizeTarget = BG;
    resizer->padding = 2;

    TextElement* playerPosText = uim->AddUI<TextElement>("PlayerPosText", this);
    playerPosText->SetTextMode(TextMode::ResizeToFit);
    playerPosText->SetWrapText(false);
    playerPosText->SetPtSize(16);
    playerPosText->SetZIndex(1);

    SetVisibility(true);
}

void DebugInfo::TickPostPhysics(float dt) {

    PlayerController* plrCtrl = GameInstance::Get()->GetPlayerController();
    Vector2 playerPos = plrCtrl->GetPlayer()->GetPosition();

    UIManager* uim = UIManager::Get();

    dynamic_cast<TextElement*>(uim->GetUI("PlayerPosText"))->SetText("Player position: " + playerPos.ToString());


}

DebugInfo::~DebugInfo() {

} 