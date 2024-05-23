#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <sstream>
#include <atlstr.h>
#include <iomanip>
#include <list>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_image.h"

using namespace std;

void InitGame();
void ClearGame();

extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern int speed;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
class PhaseInterface {
public:
	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};