#include "main-menu.hpp"
#include "raylib.h"
#include <memory>

void MainMenu::render(RenderTexture2D target, int frameCounter,
                      GameScreen *gameScreen) {

  BeginTextureMode(target);
  {
    ClearBackground(RAYWHITE);
    for (const auto &b : menu_buttons) {
      b->update();
      b->draw();
    }
  }
  EndTextureMode();

  BeginDrawing();
  {
    ClearBackground(RAYWHITE);
    DrawTexturePro(target.texture,
                   (Rectangle){0, 0, (float)target.texture.width,
                               -(float)target.texture.height},
                   (Rectangle){0, 0, (float)target.texture.width,
                               (float)target.texture.height},
                   (Vector2){0, 0}, 0.0f, WHITE);
  }
  EndDrawing();
}

void MainMenu::createButtons() {
  menu_buttons[0] = std::make_unique<Button>(
      ih, Rectangle{10.f, 10.f, 128.f, 64}, "Start",
      [&] { gs.game_screen = GameScreen::SCREEN_SHOP; });
}
