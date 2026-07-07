#include "settings.hpp"

void SettingsMenu::render(RenderTexture2D target, int frameCounter) {

  BeginTextureMode(target);
  {
    ClearBackground(RAYWHITE);
    game_volume.draw();
    game_volume.update();
    music_volume.draw();
    music_volume.update();
    sound_effects.draw();
    sound_effects.update();
    back_button.draw();
    back_button.update();
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
void SettingsMenu::setup(GameState &gamestate, InputHelper &ih) {
  gs = &gamestate;
  ih = ih;
}
