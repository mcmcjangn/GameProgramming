#include "Gamefunc.h"
#include "snake.h"

SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;
bool is_Initailized = false;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
const int FPS = 60;
const int frameDelay = 1000 / FPS;

Uint32 frameStart;
int frameTime;

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	InitGame();
	TTF_Init();
	

	g_window = SDL_CreateWindow("SNAKE GAME", 100,100,SCREEN_WIDTH,SCREEN_HEIGHT, 0);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		//하드웨어에 잘 접근하지 못했다면 -1을 리턴함
		std::cout << "Mix_OpenAudio " << Mix_GetError() << std::endl;
		exit(1);
	}
	
	Snake snake; // Snake 객체 생성

	g_flag_running = true;
	Uint32 cur_time_ms = SDL_GetTicks();


	while (g_flag_running)
	{
		frameStart = SDL_GetTicks();
		if (is_Initailized == false)
		{
			snake.Reset();
			is_Initailized = true;
		}
		snake.HandleEvents();
		snake.Update();
		snake.Render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	
	snake.~Snake();
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();

	return 0;
}