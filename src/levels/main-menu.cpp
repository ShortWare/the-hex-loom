#include "main-menu.hpp"
#include "raylib.h"
#include <memory>

#include "../tools/textureManager.h"

void MainMenu::render(RenderTexture2D target, int frameCounter,
                      GameScreen *gameScreen) {

  BeginTextureMode(target);
  {
    DrawTexture(TextureManager::GetTexture(TextureManager::MAIN_MENU_BG),0,0,WHITE);
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
      *ih, Rectangle{10.f, 10.f, 128.f, 64.f}, "Start",
      [&] { gs->game_screen = GameScreen::SCREEN_SHOP; });
  menu_buttons[1] = std::make_unique<Button>(
      *ih, Rectangle{10.f, 84.f, 128.f, 64.f}, "Settings",
      [&] { gs->game_screen = GameScreen::SCREEN_SETTINGS; });
}

void MainMenu::setup(GameState &gamestate, InputHelper &ih) {
  this->ih = &ih;
  this->gs = &gamestate;
  createButtons();
}
