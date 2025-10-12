#include "core/AssetManager.hpp"
#include <iostream>

AssetManager::~AssetManager() {
    for (auto &pair : textures_) {
        if (pair.second) {
            SDL_DestroyTexture(pair.second);
        }
    }
    textures_.clear();
}

bool AssetManager::loadTexture(SDL_Renderer* renderer, const std::string& id, const std::string& path) {
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load BMP " << path << " : " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    if (!texture) {
        std::cerr << "Failed to create texture from " << path << " : " << SDL_GetError() << std::endl;
        return false;
    }

    textures_[id] = texture;
    return true;
}

SDL_Texture* AssetManager::getTexture(const std::string& id) const {
    auto it = textures_.find(id);
    return (it != textures_.end()) ? it->second : nullptr;
}

bool AssetManager::hasTexture(const std::string& id) const {
    return textures_.find(id) != textures_.end();
}
