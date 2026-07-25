/** @file UIController.hpp */
#pragma once

#include <vector>

#include "Core/EngineSubsystem.hpp"
#include "Core/Singleton.hpp"
#include "UI/HUDController.hpp"

/**
 * UI construction chain
 * --> IHUDController pointer accessed (actually UIController in memory)
 * --> AddWidget called with desired class and ID
 * --> Widget pointer created, passed into RegisterWidget
 * --> RegisterWidget operates on overrided function (in UIController)
 * --> Widget added to ActiveWidgets
 * --> OutputManager::RegisterWidget called with widget
 * --> OutputManager creates private window and manages it
 * 
 * Resolving
 * --> Resolve calls to UIController
 * --> Resolve DOES NOT call OutputManager remove on all widgets because
 *     OutputManager resolve (called after UIController resolve) does that already
 * --> Frees widget pointers
 * --> returns resolve
 */

class Widget;

class UIController : public EngineSubsystem<UIController>, public IHUDController
{
    ENGINE_SUBSYSTEM(UIController)
    
public:
    virtual void Resolve() noexcept override;

    const std::vector<Widget*>& GetActiveWidgets();

    // add widget happens in HUDController -> calls register
    virtual void RemoveWidget(std::string UID) override;
    virtual void RegisterWidget(Widget* widget) override;
    virtual Widget* GetWidget(std::string UID) const override;

private:
    UIController();
    ~UIController();

protected:

    std::vector<Widget*> ActiveWidgets;

};
