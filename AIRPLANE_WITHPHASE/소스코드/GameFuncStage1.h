#pragma once

void Init_Stage1();
void HandleEvents_Stage1();
void Update_Stage1();
void Render_Stage1();
void Clear_Stage1();


extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern Uint32 g_frame_per_sec;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern TTF_Font* g_font;
extern SDL_Texture* g_score_texture;
extern SDL_Rect g_score_rect;
extern int g_score;

