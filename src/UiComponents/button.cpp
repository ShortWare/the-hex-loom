#include "button.hpp"
#include "raylib.h"

void Button::draw() {
  DrawRectangleRec(bounds, isHovered ? ORANGE : YELLOW);
  DrawText(button_text.c_str(), bounds.x + 10,
           bounds.y + bounds.height / 2 - 10, 20, WHITE);
}

void Button::update() {
  Vector2 mousePoint = GetMousePosition();
  isHovered = CheckCollisionPointRec(mousePoint, bounds);

  if (isHovered) {
    isPressed = ih.isButtonClicked(MOUSE_BUTTON_LEFT);

    if (ih.isButtonClicked(MOUSE_BUTTON_LEFT)) {
      if (onClick)
        onClick();
    }
  } else {
    isPressed = false;
  }
}
