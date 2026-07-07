#pragma once

#include "../UiComponents/button.hpp"
#include "../enums.h"
#include "../gamedata.hpp"
#include "../tools/inputHelper.h"
#include "raylib.h"
#include <array>
#include <memory>

class MainMenu {
public:
  MainMenu(GameState &gamestate, InputHelper &ih) : ih(ih), gs(gamestate) {
    createButtons();
  }
  void render(RenderTexture2D target, int frameCounter, GameScreen *gameScreen);

private:
  InputHelper &ih;
  GameState &gs;
  std::array<std::unique_ptr<Button>, 2> menu_buttons;
  void createButtons();
};
