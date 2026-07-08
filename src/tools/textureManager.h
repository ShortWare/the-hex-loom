//
// Created by Pd on 7/7/26.
//

#ifndef RAYLIB_GAME_TEMPLATE_TEXTUREMANAGER_H
#define RAYLIB_GAME_TEMPLATE_TEXTUREMANAGER_H

#pragma once
#include "raylib.h"
#include <array>
#include <string>

class TextureManager {
public:
    enum TextureType {
        WALL,
        WORKSHOP_BG,
        MAIN_MENU_BG,
        COUNT
    };

    static void Initialize() {

        const char* filenames[COUNT] = {
            "levels/wall.png",
            "levels/workshop_bg.png",
            "levels/main_menu_bg.png",
        };

        for (int i = 0; i < COUNT; i++) {
            std::string path = "resources/textures/" + std::string(filenames[i]);
            textures_[i] = LoadTexture(path.c_str());
        }
    }


    static Texture GetTexture(TextureType type) {
        if (type >= 0 && type < COUNT) {
            return textures_[type];
        }
        return {};
    }



    static void Shutdown() {
        for (int i = 0; i < COUNT; i++) {
            UnloadTexture(textures_[i]);
        }
    }

private:
    static std::array<Texture, COUNT> textures_;
};


#endif //RAYLIB_GAME_TEMPLATE_TEXTUREMANAGER_H
