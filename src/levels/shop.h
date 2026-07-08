//
// Created by Pd on 7/6/26.
//
#pragma once
#include "raylib.h"
#include "../enums.h"
#include "../tools/inputHelper.h"
#include "../tools/soundManager.h"
#include "../tools/roomSwitcher.h"

class Shop {


public:
    void render(RenderTexture2D target, int frameCounter, float screenWidth, float screenHeight, GameScreen* gameScreen, InputHelper inputHelper) {
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);

        DrawText("Shop", 160, 90, 120, MAROON);

        int mousePos = GetMousePosition().x;

        EndTextureMode();

        BeginDrawing();
        ClearBackground(RAYWHITE);


        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height },
            (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        EndDrawing();


        if (inputHelper.isKeyClicked(KEY_D) && !RoomSwitcher::isActive()) {
            RoomSwitcher::switchRoom(GameScreen::SCREEN_WORKSHOP, true);
            SoundManager::Play(SoundManager::MOVE);
        }


        if (RoomSwitcher::isActive()) {
            RoomSwitcher::render(gameScreen);
            return;
        }



        BeginDrawing();
        if (mousePos >= screenWidth - 20) {
            DrawRectangle(screenWidth - 20, 0, 20, screenHeight, SKYBLUE);

            if (inputHelper.isButtonClicked(0) && !RoomSwitcher::isActive()) {
                RoomSwitcher::switchRoom(GameScreen::SCREEN_WORKSHOP, true);
                SoundManager::Play(SoundManager::MOVE);
            }
        }
        EndDrawing();
    }
};