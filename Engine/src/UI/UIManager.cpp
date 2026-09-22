/** @file UIManager.cpp */

#include <fstream>

#include "UI/WidgetElement.hpp"

#include "UI/UIManager.hpp"

namespace
{
    using json = nlohmann::json;

    json loadJson(const FilePath& path) {
        if (!std::filesystem::exists(path)) {
            LOG(LogType::ERROR, "Attempt to load invalid json: {}", path.string());
            return {};
        }

        std::ifstream file{path};
        if (!file.is_open()) {
            LOG(LogType::ERROR, "Couldn't open file: {}", path.string());
            return {};
        }

        json j = json::parse(file);
        return j;
    }

    UIVector getUIVector(const json& j) {
        if (!j.is_array() ||
            j.size() != 4)
        {
            LOG(LogType::ERROR, "Unexpected json found while parsing UIVector: {}", j.dump());
            return {};
        }

        return UIVector{j[0], j[1], j[2], j[3]};
    }


}

UIManager::UIManager() {
    LOG(LogType::VITAL, "UIManager constructed");

    RootWidget = new Widget("RootWidget");

    RootWidget_H = new HTreeNode<UIElement>();
    RootWidget_H->Object = RootWidget;
    RootWidget_H->Object->SetScreenSize(UIVector(1.f, 1.f, 0.f, 0.f));

}

void UIManager::RemoveUI(const std::string& UID) {
    Recurse_Remove(UID, RootWidget_H);
}

UIElement* UIManager::GetUI(const std::string& UID, const UIHierarchy* HintParentHierarchy) {

    if (HintParentHierarchy != nullptr) {
        if (auto* ui = Recurse_Get(UID, HintParentHierarchy)) {
            return ui;
        }
        LOG(LogType::WARNING, "GetUI called with hint returned invalid ui. Falling back to root recurse");
    }

    // fallback to get from root if hint failed
    return Recurse_Get(UID, RootWidget_H);
}

void UIManager::LoadWithPreset(Widget* widget, const Preset* preset) {
    const json data = loadJson(preset->path);

    LOG(LogType::INFO, "Preset Loader: Loading preset {}", data[".name"].get<std::string>());

    Internal_ApplyPresetProperties(widget, data);

    if (!data.contains("elements")) {
        goto exit; // skip nested loop for readability
    }

    for (const auto& [key, instanceArr] : data.at("elements").items()) {
        const std::string typeName = key.substr(0, key.find("-instances"));

        LOG(LogType::INFO, "  Preset Loader: Loading elements of type {}", typeName);

        for (const json& elementJson : instanceArr) {
            const std::string name = elementJson.at(".name").get<std::string>();

            LOG(LogType::INFO, "      Preset Loader: Loading element {}", name);

            UIElement* parent = nullptr;
            if (elementJson.at(".parent") == "this") {
                parent = widget;
            } else {
                parent = GetUI(elementJson.at("parent"));
            }

            WidgetElement* elem = Internal::WidgetElementPresetRegistry::GetRegistry()[typeName](
                name,
                parent,
                elementJson
            );

        }

    }

exit:
    LOG(LogType::INFO, "Preset Loader: Loading complete");
    return;

}

void UIManager::Internal_SetZIndex(UIElement* ui, int zIndex) const {
    ui->Node->Parent->ReorderChild(ui, zOrderingPred);
}

void UIManager::Resolve() noexcept {
    LOG(LogType::VITAL, "Resolving UIManager");

    delete RootWidget_H;
}

#pragma region Recursive helpers

void UIManager::Recurse_Add(UIElement* ui, UIElement* parent, UIHierarchy* currentLevel) {
    if (parent == nullptr) {
        RootWidget_H->AddChild(ui, &ui->Node, zOrderingPred);
        return;
    }

    if ( currentLevel->Object == parent ) {
        currentLevel->AddChild(ui, &ui->Node, zOrderingPred);
        return;
    }

    for (UIHierarchy* child : currentLevel->Children) {
        Recurse_Add(ui, parent, child);
    }
}

void UIManager::Recurse_Remove(const std::string& UID, UIHierarchy* currentLevel) {

    if (currentLevel->Object->GetUID() == UID) {
        delete currentLevel;
        return;
    }

    for (UIHierarchy* child : currentLevel->Children) {
        Recurse_Remove(UID, child);
    }

}
UIElement* UIManager::Recurse_Get(const std::string& UID, const UIHierarchy* currentLevel) {
    if (currentLevel->Object->GetUID() == UID) {
        return currentLevel->Object;
    }

    for (UIHierarchy* child : currentLevel->Children) {
        UIElement* ui = Recurse_Get(UID, child);
        if (ui != nullptr) { return ui; }
    }

    return nullptr;
}

bool UIManager::zOrderingPred(const UIElement* sibling, const UIElement* insertingChild) {
    return insertingChild->GetZIndex() < sibling->GetZIndex();
}

#pragma endregion

UIManager::~UIManager() {
}
