#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>

class AssetManager {
private:
    std::unordered_map<std::string, SDL_Texture*> textures_;

public:
    ~AssetManager();

    bool loadTexture(SDL_Renderer* renderer, const std::string& id, const std::string& path);
    SDL_Texture* getTexture(const std::string& id) const;
    bool hasTexture(const std::string& id) const;
};
