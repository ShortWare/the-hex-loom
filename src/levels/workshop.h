//
// Created by Pd on 7/6/26.
//
#pragma once
#include "raylib.h"
#include "../enums.h"
#include "../tools/inputHelper.h"
#include "../gameplay/castingGrid.h"
#include "../gameplay/spells.h"
#include "../tools/soundManager.h"
#include "../tools/roomSwitcher.h"

class Workshop {
    CastingGrid* castingGrid = nullptr;

public:
    void render(RenderTexture2D target, int frameCounter, float screenWidth, float screenHeight, GameScreen* gameScreen, InputHelper inputHelper) {
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);

        DrawText("Workshop", 80, 90, 120, MAROON);

        int mousePos = GetMousePosition().x;

        EndTextureMode();


        BeginDrawing();
        ClearBackground(RAYWHITE);


        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height },
            (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        EndDrawing();

        if (inputHelper.isKeyClicked(KEY_A)) {
            RoomSwitcher::switchRoom(GameScreen::SCREEN_SHOP, false);
            SoundManager::Play(SoundManager::MOVE);
        } else if (inputHelper.isKeyClicked(KEY_D)) {
            RoomSwitcher::switchRoom(GameScreen::SCREEN_BOOK, true);
            SoundManager::Play(SoundManager::MOVE);
        }

        if (RoomSwitcher::isActive()) {
            RoomSwitcher::render(gameScreen);
            return;
        }

        if (mousePos >= screenWidth - 20) {
            DrawRectangle(screenWidth - 20, 0, 20, screenHeight, SKYBLUE);

            if (inputHelper.isButtonClicked(0)) {
                RoomSwitcher::switchRoom(GameScreen::SCREEN_BOOK, true);
                SoundManager::Play(SoundManager::MOVE);
            }
        }
        if (mousePos < 20) {
            DrawRectangle(0, 0, 20, screenHeight, SKYBLUE);

            if (inputHelper.isButtonClicked(0)) {
                RoomSwitcher::switchRoom(GameScreen::SCREEN_SHOP, false);
                SoundManager::Play(SoundManager::MOVE);
            }
        }




        // Casting

        if (IsMouseButtonDown(0)) {
            if (castingGrid == nullptr) {
                castingGrid = new CastingGrid(GetMousePosition(),150,300,570,570);
            }
            castingGrid->update(GetMousePosition());

            if (castingGrid->isFinished()) {
                auto moves = castingGrid->getMoves();
                const Spells::Spell *spell = evaluate(moves);
                emscripten_log(0, spell->getName().c_str());
                delete castingGrid;
                castingGrid = nullptr;
            }
        } else {
            if (castingGrid != nullptr) {
                if (castingGrid->isOverlapPreview()) {
                    castingGrid->commitOverlap();
                }

                if (castingGrid->getPointCount() >= 2 || !castingGrid->hasValidMoves()) {
                    auto moves = castingGrid->getMoves();
                    const Spells::Spell *spell = evaluate(moves);
                    emscripten_log(0, spell->getName().c_str());
                }

                delete castingGrid;
                castingGrid = nullptr;
            }
        }

        BeginDrawing();

        if (castingGrid != nullptr) {
            castingGrid->render();
        }

        EndDrawing();


    }
};