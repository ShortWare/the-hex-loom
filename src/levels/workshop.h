//
// Created by Pd on 7/6/26.
//
#pragma once
#include "raylib.h"
#include "../enums.h"
#include "../tools/inputHelper.h"

class Workshop {


public:
    void render(RenderTexture2D target, int frameCounter, float screenWidth, float screenHeight, GameScreen* gameScreen, InputHelper inputHelper) {
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);

        DrawText("Workshop", 80, 90, 120, MAROON);

        int pos = GetMousePosition().x;

        if (pos >= screenWidth - 20) {
            DrawRectangle(screenWidth - 20, 0, 20, screenHeight, SKYBLUE);

            if (inputHelper.isButtonClicked(0)) {
                *gameScreen = GameScreen::SCREEN_BOOK;
            }
        }
        if (pos < 20) {
            DrawRectangle(0, 0, 20, screenHeight, SKYBLUE);

            if (inputHelper.isButtonClicked(0)) {
                *gameScreen = GameScreen::SCREEN_SHOP;
            }
        }

        EndTextureMode();


        BeginDrawing();
        ClearBackground(RAYWHITE);


        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height },
            (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        EndDrawing();

        if (inputHelper.isKeyClicked(KEY_A)) {
            *gameScreen = GameScreen::SCREEN_SHOP;
        } else if (inputHelper.isKeyClicked(KEY_D)) {
            *gameScreen = GameScreen::SCREEN_BOOK;
        }
    }
};