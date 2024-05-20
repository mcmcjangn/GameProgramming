#pragma once

#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <iomanip>
#include <sstream>

extern int g_current_game_phase;


// Game Phases
const int PHASE_INTRO = 0;
const int PHASE_STAGE1 = 1;
const int PHASE_ENDING = 2;

extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

void InitGame();
void HandleEvents();
void Update();
void Render();
void ClearGame();

extern Uint32 g_frame_per_sec;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern TTF_Font* g_font;
extern SDL_Texture* g_score_texture;
extern SDL_Rect g_score_rect;
extern int g_score;
