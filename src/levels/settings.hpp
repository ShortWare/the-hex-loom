#pragma once

#include "../UiComponents/button.hpp"
#include "../UiComponents/slider.hpp"
#include "../enums.h"
#include "../gamedata.hpp"
#include "../tools/inputHelper.h"
#include "raylib.h"

class SettingsMenu {
public:
  SettingsMenu(InputHelper &ih) : ih(ih) {}
  SettingsMenu(GameState *gamestate, InputHelper &ih) : ih(ih), gs(gamestate) {}
  void render(RenderTexture2D target, int frameCounter);
  void setup(GameState &gamestate, InputHelper &ih);

private:
  InputHelper &ih;
  GameState *gs;
  Button back_button{ih, Rectangle{10.f, 720 - 64 - 200, 128.f, 64.f}, "Back",
                     [&] { gs->game_screen = GameScreen::SCREEN_MAINMENU; }};
  Slider game_volume{Rectangle{10.f, 50.f, 260.f, 32.f}, 0.5f,
                     [&](float val) { gs->game_volume.global_volume = val; }};
  Slider music_volume{Rectangle{10.f, 92.f, 260.f, 32.f}, 0.5f,
                      [&](float val) { gs->game_volume.music_volume = val; }};
  Slider sound_effects{Rectangle{10.f, 134.f, 260.f, 32.f}, 0.5f,
                       [&](float val) { gs->game_volume.sound_effects = val; }};
};
