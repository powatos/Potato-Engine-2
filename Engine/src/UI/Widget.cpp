/** @file Widget.cpp */

#include <ranges>

#include "nlohmann/json.hpp"

#include "UI/WidgetElement.hpp"

#include "Debug/Log.hpp"

#include "UI/Widget.hpp"

#include <fstream>

#include "UI/BoxElement.hpp"
#include "UI/ResizeElement.hpp"
#include "UI/TextElement.hpp"
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

Widget::Widget(const std::string& UID) : UIElement(UID) {

    SetScreenPosition(UIVector());
    SetScreenSize(UIVector(0.f, 0.f, 10.f, 10.f));

}

// throws
// void Widget::LoadWithPreset(const Preset* preset) {
//     using json = nlohmann::json;
//
//     UIManager* uim = UIManager::Get();
//
//     const json data = loadJson(preset->path);
//
//     LOG(LogType::INFO, "Loading preset: {}", data["name"].get<std::string>());
//
//     if (data.contains("position")) {
//         const UIVector uiv = getUIVector(data["position"]);
//         LOG(LogType::INFO, "Position set {}", uiv.ToStringF());
//         SetScreenPosition(uiv);
//     }
//     if (data.contains("size")) {
//         const UIVector uiv = getUIVector(data["size"]);
//         LOG(LogType::INFO, "Size set {}", uiv.ToStringF());
//
//         SetScreenSize(uiv);
//     }
//     if (data.contains("visibility")) {
//         const bool v = data["visibility"].get<bool>();
//         LOG(LogType::INFO, "Visibility set {}", v);
//
//         SetVisibility(v);
//     }
//     if (!data.contains("elements")) { return; }
//
//     const json elements = data["elements"];
//     for (const auto& [T_instances, instancesArr] : elements.items()) {
//
//         std::function<UIElement*(const std::string&, UIElement*)> instantiateElement;
//
//         if ( T_instances == "BoxElement-instances" ) {
//
//             instantiateElement = [&uim](const std::string& name, UIElement* parent) {
//                 return uim->AddUI<BoxElement>(name, parent);
//             };
//
//         } else if (T_instances == "TextElement-instances") {
//             instantiateElement = [&uim](const std::string& name, UIElement* parent) {
//                 return uim->AddUI<TextElement>(name, parent);
//             };
//         } else if (T_instances == "ResizeElement-instances") {
//             instantiateElement = [&uim](const std::string& name, UIElement* parent) {
//                 return uim->AddUI<ResizeElement>(name, parent);
//             };
//         }
//
//
//         for (const auto& inst : instancesArr) {
//
//             UIElement* parent = nullptr;
//             if (inst["parent"] == "this") { parent = this; }
//             else {
//                 parent = uim->GetUI(inst["parent"]);
//             }
//
//             if (parent == nullptr) {
//                 LOG(LogType::ERROR, "Invalid UI parent: {} for instance {}", inst["parent"].get<std::string>(), inst["name"].get<std::string>());
//                 return;
//             }
//
//             UIElement* ui = instantiateElement(inst["name"], parent);
//             // element properties ...
//         }
//
//     }
//
//
// }

Widget::~Widget() {


}