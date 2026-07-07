#pragma once
#include "enums.h"

struct GameVolume {
  float global_volume{1.f};
  float music_volume{1.f};
  float sound_effects{1.f};
};

struct GameState {
  GameScreen game_screen;
  GameVolume game_volume;
};
