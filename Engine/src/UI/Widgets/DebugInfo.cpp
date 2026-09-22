/** @file DebugInfo.cpp */

#include "Core/GameInstance.hpp"
#include "UI/TextElement.hpp"

#include "../../../include/Widgets/DebugInfo.hpp"

#include "Core/AssetManager.hpp"
#include "UI/BoxElement.hpp"
#include "UI/ResizeElement.hpp"
#include "UI/UIManager.hpp"

void DebugInfo::Setup() {
    Widget::Setup();

    UIManager* uim = UIManager::Get();

    Preset* preset = AssetManager::Get()->GetAsset<Preset>("DebugInfo.json");
    uim->LoadWithPreset(this, preset);

}

void DebugInfo::TickPostPhysics(float dt) {

    PlayerController* plrCtrl = GameInstance::Get()->GetPlayerController();
    Vector2 playerPos = plrCtrl->GetPlayer()->GetPosition();

    UIManager* uim = UIManager::Get();

    dynamic_cast<TextElement*>(uim->GetUI("PlayerPosText", Node))->SetText("Player position: " + playerPos.ToString());


}

DebugInfo::~DebugInfo() {

} 