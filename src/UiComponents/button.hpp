#pragma once

#include "raylib.h"
#include "../tools/inputHelper.h"
#include <functional>
#include <string>

class Button {
public:
  Button(InputHelper& ih, Rectangle rec, std::string text, std::function<void()> callback)
      : ih(ih), bounds(rec), button_text(text), onClick(callback) {}
  void draw();
  void update();

private:
  Rectangle bounds;
  std::string button_text;
  std::function<void()> onClick;
  InputHelper& ih;

  bool isHovered{false};
  bool isPressed{false};
};
