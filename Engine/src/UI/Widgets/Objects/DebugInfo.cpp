/** @file DebugInfo.cpp */

#include "Core/GameInstance.hpp"
#include "UI/TextElement.hpp"

#include "UI/DebugInfo.hpp"

DebugInfo::DebugInfo(std::string UID) : Widget(std::move(UID)) {
    
    TextElement* playerPosElem = AddElement<TextElement>("PlayerPosText");
    TextElement* playerVelElem = AddElement<TextElement>("PlayerVelText");
    TextElement* playerRotElem = AddElement<TextElement>("PlayerRotText");

    playerPosElem->SetScreenPosition(Vector2(0.0,0.0));
    playerVelElem->SetScreenPosition(Vector2(0.0,1.0));
    playerRotElem->SetScreenPosition(Vector2(0.0,2.0));

    playerPosElem->SetVisibility(true);
    playerVelElem->SetVisibility(true);
    playerRotElem->SetVisibility(true);

    SetScreenSize(Vector2(40.0, 5.0));
    SetScreenPosition(Vector2(0.0, 0.0));

    SetVisibility(true);
}

void DebugInfo::TickPostPhysics(float dt) {
    Widget::Tick(dt);

    PlayerController* plrCtrl = GameInstance::Get()->GetPlayerController();
    Vector2 playerPos = plrCtrl->GetPlayer()->GetPosition();
    Vector2 playerVel = plrCtrl->GetPlayer()->GetVelocity();
    // float playerRot = plrCtrl->GetPlayer()->GetTexture().GetRotation();
    
    dynamic_cast<TextElement*>(GetElement("PlayerPosText"))->field = "Player position: " + playerPos.ToString();
    dynamic_cast<TextElement*>(GetElement("PlayerVelText"))->field = "Player velocity: " + playerVel.ToStringF();
    // dynamic_cast<TextElement*>(GetElement("PlayerRotText"))->field = "Player rotation: " + std::to_string(playerRot);

}

DebugInfo::~DebugInfo() {

} 