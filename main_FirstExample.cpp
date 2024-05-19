#include "GameFunc.h"
#include "GameFuncIntro.h"
#include "GameFuncStage1.h"
#include "GameFuncEnding.h"

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
    Init_Ending();

	g_current_game_phase = PHASE_INTRO;

	g_last_time_ms = SDL_GetTicks();

    static bool is_intro_initialized = false;
    static bool is_stage1_initialized = false;
    static bool is_ending_initialized = false;

    while (g_flag_running)
    {
        Uint32 cur_time_ms = SDL_GetTicks();
        if (cur_time_ms - g_last_time_ms < 33)
            continue;

        if (g_current_game_phase == PHASE_INTRO)
        {
            // PHASE_ENDING에서 PHASE_INTRO로 전환될 때 Clear_Ending()과 Init_Intro() 호출
           
            if (!is_intro_initialized) {
                Clear_Ending();
                Init_Intro();
                is_intro_initialized = true;
                is_ending_initialized = false;
            }
            HandleEvents_Intro();
            Update_Intro();
            Render_Intro();
        }
        else if (g_current_game_phase == PHASE_STAGE1)
        {
            // PHASE_STAGE1로 바뀌었을 때 Init_Stage1() 호출
            
            if (!is_stage1_initialized) {
                Clear_Intro();
                Init_Stage1();
                is_stage1_initialized = true;
                is_intro_initialized = false;
            }

            HandleEvents_Stage1();
            Update_Stage1();
            Render_Stage1();
        }
        else if (g_current_game_phase == PHASE_ENDING)
        {
            // PHASE_ENDING으로 바뀌었을 때 Init_Ending() 호출
           
            if (!is_ending_initialized) {
                Clear_Stage1();
                Init_Ending();
                is_ending_initialized = true;
                is_stage1_initialized = false;
            }
            HandleEvents_Ending();
            Update_Ending();
            Render_Ending();


        }

        g_last_time_ms = cur_time_ms;
    }



	Clear_Intro();
	Clear_Stage1();
	Clear_Ending();
	ClearGame();

	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();

	return 0;
}
