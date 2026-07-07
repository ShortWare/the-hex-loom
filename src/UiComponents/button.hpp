#pragma once
#include "../tools/inputHelper.h"
#include "raylib.h"
#include <functional>
#include <string>

struct ButtonTexture {
  Texture2D button;
  Texture2D button_hovered;
};

class Button {
public:
  // No Texture
  Button(InputHelper &ih, Rectangle rec, std::string text,
         std::function<void()> callback)
      : ih(ih), bounds(rec), button_text(text), onClick(callback) {}
  // Texture with text
  Button(InputHelper &ih, Rectangle rec, ButtonTexture txt, std::string text,
         std::function<void()> callback)
      : ih(ih), bounds(rec), button_text(text), onClick(callback),
        isTexture(true), button_textures(txt) {}
  // Texture without text
  Button(InputHelper &ih, Rectangle rec, ButtonTexture txt,
         std::function<void()> callback)
      : ih(ih), bounds(rec), onClick(callback), isTexture(true),
        button_textures(txt) {}
  void draw();
  void update();

private:
  Rectangle bounds;
  std::string button_text{};
  std::function<void()> onClick;
  InputHelper &ih;

  bool isTexture{false};
  ButtonTexture button_textures;

  bool isHovered{false};
  bool isPressed{false};
};
