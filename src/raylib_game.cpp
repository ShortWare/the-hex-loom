/*******************************************************************************************
 *
 *   raylib gamejam template
 *
 *   Code licensed under an unmodified zlib/libpng license, which is an
 * OSI-certified, BSD-like license that allows static linking with closed source
 * software
 *
 *   Copyright (c) 2022-2026 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "UiComponents/button.hpp"
#include "enums.h"
#include "gamedata.hpp"
#include "levels/book.h"
#include "levels/settings.hpp"
#include "levels/shop.h"
#include "raylib.h"
#include "tools/inputHelper.h"

#if defined(PLATFORM_WEB)
#include "tools/soundManager.h"
#include <emscripten/emscripten.h> // Emscripten library
#endif

#include <stdio.h>  // Required for: printf()
#include <stdlib.h> // Required for:
#include <string.h> // Required for:

#include "levels/main-menu.hpp"
#include "levels/workshop.h"

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// TODO: Define your custom data types here

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 720;
static const int screenHeight = 720;

static RenderTexture2D target = {0}; // Render texture to render our game
static int frameCounter = 0;

static bool wasMouseLeftDown = false;

static GameState gamestate{.game_screen = GameScreen::SCREEN_MAINMENU};

Book book = Book();
Shop shop = Shop();
Workshop workshop = Workshop();

InputHelper inputHelper = InputHelper();

MainMenu main_menu{};
SettingsMenu settings_menu{inputHelper};
// TODO: Define global variables here, recommended to make them static

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void); // Update and Draw one frame

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
#if !defined(_DEBUG)
  SetTraceLogLevel(LOG_NONE); // Disable raylib trace log messages
#endif

  // Initialization
  //--------------------------------------------------------------------------------------
  InitWindow(screenWidth, screenHeight, "raylib gamejam template");

  // TODO: Load resources / Initialize variables at this point

  // Render texture to draw, enables screen scaling
  // NOTE: If screen is scaled, mouse input should be scaled proportionally
  target = LoadRenderTexture(screenWidth, screenHeight);
  SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

  SoundManager::Initialize();
  main_menu.setup(gamestate, inputHelper);
  settings_menu.setup(gamestate, inputHelper);

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
  SetTargetFPS(60); // Set our game frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button
  {
    UpdateDrawFrame();
  }
#endif

  // De-Initialization
  //--------------------------------------------------------------------------------------
  UnloadRenderTexture(target);

  SoundManager::Shutdown();

  // TODO: Unload all loaded resources at this point

  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

//--------------------------------------------------------------------------------------------
// Module Functions Definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void) {
  bool clickedLeft = false;
  // Update
  //----------------------------------------------------------------------------------
  // TODO: Update variables / Implement example logic at this point

  frameCounter++;

  inputHelper.update();

  switch (gamestate.game_screen) {
  case GameScreen::SCREEN_BOOK:
    book.render(target, frameCounter, screenWidth, screenHeight,
                &gamestate.game_screen, inputHelper);
    break;
  case GameScreen::SCREEN_SHOP:
    shop.render(target, frameCounter, screenWidth, screenHeight,
                &gamestate.game_screen, inputHelper);
    break;
  case GameScreen::SCREEN_WORKSHOP:
    workshop.render(target, frameCounter, screenWidth, screenHeight,
                    &gamestate.game_screen, inputHelper);
    break;
  case GameScreen::SCREEN_MAINMENU:
    main_menu.render(target, frameCounter, &gamestate.game_screen);
    break;
  case GameScreen::SCREEN_MARKET:
    break;
  case GameScreen::SCREEN_SETTINGS:
    settings_menu.render(target, frameCounter);
    break;
  }
}
