//
// Created by Pd on 7/6/26.
//

#include "raylib.h"

#ifndef RAYLIB_GAME_TEMPLATE_INPUTHELPER_H
#define RAYLIB_GAME_TEMPLATE_INPUTHELPER_H


class InputHelper {
    bool wasButtonDown[3] = {};
    bool buttonClicked[3] = {};

    bool wasKeyDown[337] = {};
    bool keyClicked[337] = {};

public:
    void update() {
        for (int i = 0; i < 3; i++) {
            if (IsMouseButtonDown(i)) {
                wasButtonDown[i] = true;
            } else {
                buttonClicked[i] = wasButtonDown[i];
                wasButtonDown[i] = false;
            }
        }

        for (int i = 0; i < 337; i++) {
            if (IsKeyDown(i)) {
                wasKeyDown[i] = true;
            } else {
                keyClicked[i] = wasKeyDown[i];
                wasKeyDown[i] = false;
            }
        }
    }

    bool isButtonClicked(int button) {
        return buttonClicked[button];
    }

    bool isKeyClicked(int key) {
        return keyClicked[key];
    }
};

#endif //RAYLIB_GAME_TEMPLATE_INPUTHELPER_H
