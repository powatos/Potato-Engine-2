/** @file "TextureManager.hpp" */
#pragma once

#include <array>
#include <unordered_map>

#include "EngineSubsystem.hpp"
#include "Texture.hpp"

/**
 * @brief Manager to interface global textures
 * @details The following file types are supported for image textures: @sa validTextures
 * - BMP (.bmp)
 * - <del>PNG (.png)</del>
 * - <del>JPEG (.jpg / .jpeg)</del>
 */
class TextureManager : public EngineSubsystem<TextureManager>
{
    ENGINE_SUBSYSTEM(TextureManager)

public:
    TextureManager();
    void Resolve() noexcept override;

    /**
     * @brief Gets a texture based on its corresponding file
     * @param texturePath Path from textures folder to the file associated with the texture
     * @returns Texture associated with file
     */
    Texture* GetTexture(const std::string& relativePath);

    /**
     * @brief Creates a new texture object based on its corresponding file
     * @details All assets under the assets folder are automatically created and cached on engine startup. Use this function to manually create a texture
     * with a custom file path. Created textures are always cached locally and can be fetched at any time using `GetTexture()`
     * @warning It is not recommended to create textures manually. Instead, place all assets under the default assets folder to automatically create texture
     * refs based on file type
     * @param rootPath Path from OS default home directory to file
     * @returns Texture object constructed from the file
     */
    Texture* CreateTexture(const FilePath& rootPath);

    static constexpr std::array<std::string_view, 1> validExtensions {
        ".bmp"
    };


private:

    std::unordered_map<std::string, Texture*> cache;
};

