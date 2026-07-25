/** @file TextureManager.hpp */
#pragma once

#include <unordered_map>

#include "Texture.hpp"

#include "Core/Singleton.hpp"
#include "Core/EngineSubsystem.hpp"

/**
 * @brief Manager class for handling actor textures
 */
class TextureManager : public EngineSubsystem<TextureManager>
{
    ENGINE_SUBSYSTEM(TextureManager)

public:
    virtual void BeginPlay() override;
    virtual void Resolve() noexcept override;

    /**
     * @brief Registers a texture that can be used by the engine
     * @remark All textures placed under the default Textures directory are automatically registered by
     * file name on engine startup. Use this function to register textures at runtime or from other directories
     * @param name Name of texture
     * @param texture Texture object to register
     */
    void RegisterTexture(const std::string& textureFileName);
    /**
     * @brief Gets a registered texture by name
     * @param name Name of texture to get
     * @returns Texture object reference
     */
    const Texture& GetTexture(const std::string& name);
    /**
     * @brief Checks if a texture is registered
     * @param name Name of texture to check
     * @returns true if texture is registered
     */
    bool HasTexture(const std::string& name) const;

protected:
    TextureManager();
    ~TextureManager();

private:
    std::unordered_map<std::string, Texture> textures;

};