#include "GameFunc.h"

SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {

	// Initializing SDL library
	//오디오 장치 준비
	SDL_Init(SDL_INIT_EVERYTHING);
	//ttf init
	TTF_Init();

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		//하드웨어에 잘 접근하지 못했다면 -1을 리턴함
		std::cout << "Mix_OpenAudio " << Mix_GetError() << std::endl;
		exit(1);
	}

	g_window = SDL_CreateWindow("202221226", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);
	InitGame();

	g_last_time_ms = SDL_GetTicks();

	while (g_flag_running) {

		Uint32 cur_time_ms = SDL_GetTicks();

		if (cur_time_ms - g_last_time_ms < 33)
			continue;

		HandleEvents();
		Update();
		Render();

		// SDL_Delay(30);	// 30 밀리세컨드 기다린다.
		g_last_time_ms = cur_time_ms;
	}
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	ClearGame();
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();

	return 0;
}
