#pragma once
#include "SDL_ttf.h"
#include "SDL_mixer.h"

void Init_Intro();
void HandleEvents_Intro();
void Update_Intro();
void Render_Intro();
void Clear_Intro();



extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern Uint32 g_frame_per_sec;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern TTF_Font* g_font;
extern SDL_Texture* g_score_texture;
extern SDL_Rect g_score_rect;
extern int g_score;