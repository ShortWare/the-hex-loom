//
// Created by Pd on 7/7/26.
//

#ifndef RAYLIB_GAME_TEMPLATE_SOUNDMANAGER_H
#define RAYLIB_GAME_TEMPLATE_SOUNDMANAGER_H


#pragma once
#include "raylib.h"
#include <array>
#include <string>

class SoundManager {
public:
    enum SoundType {
        MOVE,
        COUNT
    };

    static void Initialize() {
        if (!IsAudioDeviceReady()) {
            InitAudioDevice();
        }

        const char* filenames[COUNT] = {
            "move.wav"
        };

        for (int i = 0; i < COUNT; i++) {
            std::string path = "resources/sounds/" + std::string(filenames[i]);
            sounds_[i] = LoadSound(path.c_str());
        }
    }

    static void Play(SoundType type) {
        if (type >= 0 && type < COUNT) {
            PlaySound(sounds_[type]);
        }
    }

    static bool IsPlaying(SoundType type) {
        if (type >= 0 && type < COUNT) {
            return IsSoundPlaying(sounds_[type]);
        }
        return false;
    }

    static void Shutdown() {
        for (int i = 0; i < COUNT; i++) {
            UnloadSound(sounds_[i]);
        }
        CloseAudioDevice();
    }

private:
    static std::array<Sound, COUNT> sounds_;
};


#endif //RAYLIB_GAME_TEMPLATE_SOUNDMANAGER_H
