/** @file "LoadingScreen.hpp" */
#pragma once
#include "UI/Widget.hpp"


class LoadingScreen : public Widget
{
public:
    LoadingScreen(const std::string& UID);

    void Setup() override;

    void Increment();

private:
    unsigned int progress;

};
