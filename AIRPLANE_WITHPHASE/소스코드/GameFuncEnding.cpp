#include "GameFunc.h"
#include "GameFuncEnding.h"
static SDL_Rect g_text_rect_ending;
SDL_Texture* g_ending_image;
Mix_Music* g_bg_mus;
TTF_Font* endingFont;
SDL_Color endingFontColor;
SDL_Surface* endingSurface;
SDL_Texture* endingTexture;
void Init_Ending()
{

    // 엔딩 이미지 로드
    SDL_Surface* ending_surface = IMG_Load("../resource/ending.jpg");
    if (ending_surface == NULL) {
        std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
        exit(1);
    }
    g_ending_image = SDL_CreateTextureFromSurface(g_renderer, ending_surface);
    SDL_FreeSurface(ending_surface);


    //배경음악 로드
    g_bg_mus = Mix_LoadMUS("../resource/bgbgm.mp3");
    if (g_bg_mus == NULL) {
        std::cout << "Mix_LoadMUS: " << Mix_GetError() << std::endl;
        exit(1);
    }

    Mix_HaltMusic();
    Mix_PlayMusic(g_bg_mus, -1);

}

void Update_Ending()
{
}

void Render_Ending()
{


    // 엔딩 이미지 그리기
    SDL_Rect ending_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(g_renderer, g_ending_image, NULL, &ending_rect);
    // 화면 업데이트
    SDL_RenderPresent(g_renderer);
}

void HandleEvents_Ending()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            g_flag_running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            // If the mouse left button is pressed. 
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                g_current_game_phase = PHASE_INTRO;
            }
            break;

        default:
            break;
        }
    }
}

void Clear_Ending()
{
    SDL_DestroyTexture(g_ending_image);
    Mix_HaltMusic();
    Mix_FreeMusic(g_bg_mus);

}