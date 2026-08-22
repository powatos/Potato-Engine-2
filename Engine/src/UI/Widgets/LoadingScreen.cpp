/** @file "LoadingScreen.cpp" */

#include "Widgets/LoadingScreen.hpp"

LoadingScreen::LoadingScreen(const std::string& UID) : Widget(UID) {
    progress = 0u;
}

void LoadingScreen::Setup() {
    Widget::Setup();


}
