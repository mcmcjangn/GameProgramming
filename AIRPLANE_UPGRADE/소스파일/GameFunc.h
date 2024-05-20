#pragma once

#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <iomanip>
#include <sstream>

void InitGame();
void HandleEvents();
void Update();
void Render();
void ClearGame();

extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern Uint32 g_frame_per_sec;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern TTF_Font* g_font;
extern SDL_Texture* g_score_texture;
extern SDL_Rect g_score_rect;
extern int g_score;
