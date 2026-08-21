/** @file UIManager.hpp */
#pragma once

#include <vector>
#include <nlohmann/json.hpp>
#include <boost/callable_traits.hpp>

#include "Core/EngineSubsystem.hpp"
#include "Core/Singleton.hpp"

#include "Widget.hpp"
#include "Debug/Log.hpp"

namespace Internal
{
/**
 * @def REGISTER_WIDGETELEMENT(type)
 * @brief Registers a widget element to be instantiated via preset
 * @param type Literal type of widget element
 */
#define REGISTER_WIDGETELEMENT(type) \
static struct type##_RegisterWidgetElement { \
    type##_RegisterWidgetElement() { \
        Internal::WidgetElementPresetRegistry::GetRegistry()[#type] = [](const std::string& name, UIElement* parent, const nlohmann::json& data) -> WidgetElement* { \
            UIManager* uim = UIManager::Get(); \
            type* e = uim->AddUI<type>(name, parent); \
            uim->Internal_ApplyPresetProperties(e, data); \
            return e; \
        }; \
    } \
} type##_RegisterWidgetElement_i;

}

class UIManager : public EngineSubsystem<UIManager>
{
    ENGINE_SUBSYSTEM(UIManager)
    
public:
    virtual void Resolve() noexcept override;

    UIHierarchy* RootWidget_H;
    Widget* RootWidget;

    /**
     * @brief Construct and add UI parented to another UI onto the screen
     * @note Widgets may only be parented under other widgets, but WidgetElements may follow any hierarchical order
     * * @tparam UIClass Widget or WidgetElement type to add
     * @param UID Unique ID to identify UI
     * @param parent UI to parent this UI to
     * @returns UI that was added
     */
    template<std::derived_from<UIElement> UIClass, std::derived_from<UIElement> Parent = UIElement>
    UIClass* AddUI(const std::string& UID, Parent* parent);
    /**
     * @brief Construct and add a base widget (parented to the root widget)
     * * @tparam WidgetClass Widget type to add
     * @param UID Unique ID to identify UI
     * @returns Widget that was added
     */
    template<std::derived_from<Widget> WidgetClass>
    WidgetClass* AddUI(const std::string& UID);

    /**
     * @brief Remove a Widget from the screen
     * @param UID UID of the widget to remove
     * @note This function deletes the widget instance and render buffer. Do not use this
     * to hide the widget temporarily, or when preservation of data is vital. @sa Widget::SetVisibility
     */
    void RemoveUI(const std::string& UID);

    /** @brief Gets UI by UID
     * @param UID UID
     * @param HintParent Parent of target UI; optional if unknown
     * @returns UI matching name
     */
    UIElement* GetUI(const std::string& UID, const UIElement* HintParent = nullptr);

    /**
     * @brief Loads a widget object using a preset
     * @param widget widget to load onto
     * @param preset Preset to load
     */
    void LoadWithPreset(Widget* widget, const Preset* preset);


    void Internal_SetZIndex(UIElement* ui, int zIndex) const;

    template<typename UIClass>
    requires std::derived_from<UIClass, Widget> || std::derived_from<UIClass, WidgetElement>
    void Internal_ApplyPresetProperties(UIClass* ui, const nlohmann::json& data);

private:
    UIManager();
    ~UIManager();

    void Recurse_Add(UIElement* ui, UIElement* parent, UIHierarchy* currentLevel);
    void Recurse_Remove(const std::string& UID, UIHierarchy* currentLevel);
    UIElement* Recurse_Get(const std::string& UID, const UIHierarchy* currentLevel);

    static bool zOrderingPred(const UIElement* sibling, const UIElement* insertingChild);


};

template<std::derived_from<UIElement> UIClass, std::derived_from<UIElement> Parent>
UIClass* UIManager::AddUI(const std::string& UID, Parent* parent) {
    static_assert(
        !std::derived_from<UIClass, Widget> ||
        std::derived_from<Parent, Widget>,
        "Widgets may only be parented to other widgets!"
    ); // keep static assertion instead of concept for readability

    UIClass* ui = new UIClass(UID);

    Recurse_Add(ui, parent, RootWidget_H);
    ui->Setup();

    return ui;
}

template<std::derived_from<Widget> WidgetClass>
WidgetClass* UIManager::AddUI(const std::string& UID) {
    WidgetClass* ui = AddUI<WidgetClass>(UID, RootWidget);
    return ui;
}

namespace Internal::WidgetElementPresetRegistry
{
    using RegistryType = std::unordered_map<std::string, std::function<WidgetElement*(const std::string&, UIElement*, const nlohmann::json& data)>>;

    inline RegistryType& GetRegistry() {
        static RegistryType registry;
        return registry;
    }
}

template<>
struct nlohmann::adl_serializer<UIVector> {
    static UIVector from_json(const json& j) {
        return UIVector{
            j.at(0).get<float>(),
            j.at(1).get<float>(),
            j.at(2).get<float>(),
            j.at(3).get<float>()
        };
    }
    static void to_json(json& j, const UIVector& v) {
        j = { v.scale.x, v.scale.y, v.offset.x, v.offset.y};
    }
};
template<>
struct nlohmann::adl_serializer<Color> {
    static Color from_json(const json& j) {
        return Color{
            static_cast<unsigned char>(j.at(0).get<int>()),
            static_cast<unsigned char>(j.at(1).get<int>()),
            static_cast<unsigned char>(j.at(2).get<int>()),
            static_cast<unsigned char>(j.at(3).get<int>())
        };
    }
    static void to_json(json& j, const Color& v) {
        j = {v.R, v.G, v.B, v.A};
    }
};
template<>
struct nlohmann::adl_serializer<UIElement*> {
    static UIElement* from_json(const json& j) {
        return UIManager::Get()->GetUI(j.get<std::string>());
    }
    // write to json using UIElement* should never occur
};

template<typename UIClass>
requires std::derived_from<UIClass, Widget> || std::derived_from<UIClass, WidgetElement>
void UIManager::Internal_ApplyPresetProperties(UIClass* ui, const nlohmann::json& data) {
    namespace bd = boost::describe;
    using Members_fn = bd::describe_members<UIClass,
        bd::mod_public |
        bd::mod_inherited |
        bd::mod_function
    >;
    using Members_data = bd::describe_members<UIClass,
        bd::mod_public |
        bd::mod_inherited
    >;

    const std::string uiName = data[".name"].get<std::string>();

    boost::mp11::mp_for_each<Members_fn>([&](const auto& descriptor) {
        if (!data.contains(descriptor.name)) { return; }

        using ptrType = decltype(descriptor.pointer);

        static_assert(std::is_member_function_pointer_v<ptrType>);
        // setter

        // first element of tuple contains object pointer
        using ptrArgs = boost::callable_traits::args_t<ptrType>;
        static_assert(std::tuple_size_v<ptrArgs> == 2, "Only single-argument setters are supported for preset setters");

        using ArgT = std::remove_cvref_t<std::tuple_element_t<1, ptrArgs>>;
        const ArgT arg = data.at(descriptor.name).template get<ArgT>();

        (ui->*descriptor.pointer)(arg);

        LOG(LogType::INFO, "        Preset Loader: Mutated {}->{}(...);", uiName, descriptor.name);

    });

    // public member
    boost::mp11::mp_for_each<Members_data>([&](const auto& descriptor) {
        if (!data.contains(descriptor.name)) { return; }

        using ptrType = decltype(descriptor.pointer);

        static_assert(!std::is_member_function_pointer_v<ptrType>);

        using MemberType = std::remove_reference_t<decltype(ui->*descriptor.pointer)>;
        const MemberType set = data.at(descriptor.name).template get<MemberType>();

        ui->*descriptor.pointer = set;

        LOG(LogType::INFO, "        Preset Loader: Mutated {}->{} = ...;", uiName, descriptor.name);

    });

}
