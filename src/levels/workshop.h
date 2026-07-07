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

class Workshop {
    CastingGrid* castingGrid = nullptr;

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
            SoundManager::Play(SoundManager::MOVE);
        } else if (inputHelper.isKeyClicked(KEY_D)) {
            *gameScreen = GameScreen::SCREEN_BOOK;
            SoundManager::Play(SoundManager::MOVE);
        }




        // Casting

        if (IsMouseButtonDown(0)) {
            if (castingGrid == nullptr) {
                castingGrid = new CastingGrid(GetMousePosition());
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