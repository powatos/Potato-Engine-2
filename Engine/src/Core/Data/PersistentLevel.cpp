/** @file PersistentLevel.cpp */

#include "Core/Archivable.hpp"
#include "Core/GameInstance.hpp"
#include "Game/World.hpp"
#include "nlohmann/json.hpp"

#include "Debug/Log.hpp"
#include "Util/Vector2.hpp"

#include "Core/PersistentLevel.hpp"

using json = nlohmann::json;

static json safeGetJson(const path& path);

PersistentLevel::PersistentLevel(const std::string& saveFileName) {

    saveFileAbsPath = DataManager::GetSaveDir() / saveFileName;
    LOG(LogType::INFO, "Persistent level initialized with save file: {}", saveFileAbsPath.string());
}   

bool PersistentLevel::LoadStaticActors() {
    json Save = safeGetJson(saveFileAbsPath);
    json StaticActors = Save["StaticActors"];

    bool allSuccessful = true;

    World* world = GameInstance::Get()->GetWorld();
    ___ArchiveType& Archive = ___Archive::_GetArchive();

    LOG(LogType::INFO, "Loading static actors from persistent level");

    for (const auto& [ClassID, posList] : StaticActors.items()) {
        
        auto it = Archive.find(ClassID);
        // if ClassID not found in archive, continue to skip
        if (it == Archive.end()) {
            LOG(LogType::WARNING, "Unkown ClassID in factory: {} - skipped loading", ClassID);
            continue;
        }

        auto constructor = it->second;
        // if constsructor not found, continue to skip
        if (constructor == nullptr) {
            LOG(LogType::WARNING, "Bad function reference in archive for class: {} - skipped loading", ClassID);
            continue;
        }
        
        LOG(LogType::INFO, "Loading static actors of class: {}", ClassID);

        for (const auto& pos : posList) {
            
            if (!pos.is_array() || pos.size() != 2) {
                LOG(LogType::WARNING, "Invalid position data for class: {} - skipped loading", ClassID);
                continue;
            } 

            Vector2 Position;

            try {

                float x = pos[0].get<float>();
                float y = pos[1].get<float>();
                Position = Vector2(x,y);
                
            } catch (const json::type_error& e) {

                LOG(LogType::WARNING, "Failed to parse data for actor: {} - skipped loading", e.what());
                allSuccessful = false;

                continue;
            }
            
            Archivable* obj = constructor();

            // if returns nullptr, skip
            if (obj == nullptr) {
                LOG(LogType::WARNING, "Object could not be constructor for class: {} - skipped loading", ClassID);
                allSuccessful = false;
                continue;
            }

            if (Actor* addedActor = world->AddtoPool(dynamic_cast<Actor*>(obj))) {
                addedActor->SetPosition(Position);
            } else {
                allSuccessful = false;
                LOG(LogType::WARNING, "Failed to load static actor when adding to actor pool: {} - skipped loading", ClassID);
                delete obj;
            }
        }

    }


    LOG(LogType::INFO, "Static actors loaded {}", allSuccessful ? "successfully" : "unsuccessfully");
    return allSuccessful;
}

#pragma region Data getters and writers

int PersistentLevel::GetIntData( std::string key ) const {
    json Save = safeGetJson(saveFileAbsPath);

    return Save["Data"][key].get<int>();
}
float PersistentLevel::GetFloatData( std::string key ) const {
    json Save = safeGetJson(saveFileAbsPath);

    return Save["Data"][key].get<float>();
}
std::string PersistentLevel::GetStringData( std::string key ) const {
    json Save = safeGetJson(saveFileAbsPath);

    return Save["Data"][key].get<std::string>();
}
Vector2 PersistentLevel::GetVector2Data( std::string key ) const {
    json Save = safeGetJson(saveFileAbsPath);

    json vecJson = Save["Data"][key];
    return Vector2(vecJson[0].get<float>(), vecJson[1].get<float>());
}

void PersistentLevel::WriteIntData( std::string key, int value ) {
    json Save = safeGetJson(saveFileAbsPath);

    Save["Data"][key] = value;
}
void PersistentLevel::WriteFloatData( std::string key, float value ) {
    json Save = safeGetJson(saveFileAbsPath);

    Save["Data"][key] = value;
}
void PersistentLevel::WriteStringData( std::string key, std::string value ) {
    json Save = safeGetJson(saveFileAbsPath);

    Save["Data"][key] = value;
}
void PersistentLevel::WriteVector2Data( std::string key, const Vector2& value ) {
    json Save = safeGetJson(saveFileAbsPath);

    Save["Data"][key] = { value.x, value.y };
}

#pragma endregion

static json safeGetJson(const path& savePath) {
    json parsed;

    const bool bFileEmpty = std::filesystem::exists(savePath) && std::filesystem::file_size(savePath) == 0;
    if (!std::filesystem::exists(savePath) || bFileEmpty) {
        LOG(LogType::WARNING, "Save file not found at {}. Copying from fallback", savePath.string());
        
        if (bFileEmpty) {
            std::filesystem::remove(savePath);
        }

        std::ofstream outFile(savePath);
        try {
            parsed = json::parse(PersistentLevel::fallbackSaveString);
            if (outFile.is_open()) {
                outFile << parsed.dump(4);
                outFile.close();
            }
            LOG(LogType::INFO, "Fallback save file copied to {}", savePath.string());
        } catch (const json::parse_error& e) {
            LOG(LogType::ERROR, "Failed to parse fallback save string: {}", e.what());
        }
    }

    std::ifstream file(savePath);
    if (!file.is_open()) {
        LOG(LogType::ERROR, "Could not open save file at {}", savePath.string());
        return parsed;
    }

    try {
        parsed = json::parse(file);
    } catch (const json::parse_error& e) {
        LOG(LogType::ERROR, "Failed to parse save file at {}: {}", savePath.string(), e.what());
    }

    return parsed;

}







