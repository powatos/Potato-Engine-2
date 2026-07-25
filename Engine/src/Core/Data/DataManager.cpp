/** @file DataManager.cpp */

#include "Core/PotatoEngine.hpp"

#include "Debug/Log.hpp"

#include "SDL3/SDL.h"

#include "Core/DataManager.hpp"

path DataManager::GetConfigDir() {
    const char* basePath = SDL_GetBasePath();
    path configDir = path(basePath) / "Config";

    return configDir;
}

path DataManager::GetSaveDir() {
    char* prefPath = SDL_GetPrefPath(__ENGINE_GLOBALS::orgName.c_str() ,__ENGINE_GLOBALS::appName.c_str());
    path saveDir = path(prefPath) / "Saves";
    SDL_free(prefPath);

    return saveDir;
}
