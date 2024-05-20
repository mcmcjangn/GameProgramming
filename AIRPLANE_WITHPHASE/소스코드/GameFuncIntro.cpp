#include "GameFunc.h"
#include "GameFuncIntro.h"

static SDL_Texture* g_texture_intro; // the SDL_Texture 
static SDL_Rect g_source_rectangle_intro; // the rectangle for source image
static SDL_Rect g_destination_rectangle_intro; // for destination

SDL_Rect g_start_button_rect;

static TTF_Font* g_font_intro; // the TTF_Font
static SDL_Texture* g_text_texture_intro; // the SDL_Texture for text
static SDL_Rect g_text_rect_intro; // the rectangle for text

Mix_Music* g_music_intro;

void Init_Intro()
{
	
	//BG
	SDL_Surface* bg_surface = IMG_Load("../resource/Intro.jpg");
	g_texture_intro = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	//SDL_Texture* bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);
	SDL_QueryTexture(g_texture_intro, NULL, NULL, &g_source_rectangle_intro.w, &g_source_rectangle_intro.h);
	g_destination_rectangle_intro.x = g_source_rectangle_intro.x = 0;
	g_destination_rectangle_intro.y = g_source_rectangle_intro.y = 0;
	g_destination_rectangle_intro.w = 400;
	g_destination_rectangle_intro.h = 600;
	// TTF 폰트 로드
	TTF_Init();
	g_font_intro = TTF_OpenFont("../resource/Mor.ttf", 100); // 폰트 파일 경로와 크기를 적절히 설정하세요.

	// "CLICK TO START" 텍스트 텍스처 생성
	SDL_Surface* text_surface = TTF_RenderText_Solid(g_font_intro, "CLICK TO START", { 255, 255, 0 });
	g_text_texture_intro = SDL_CreateTextureFromSurface(g_renderer, text_surface);
	SDL_FreeSurface(text_surface);

	// 텍스트 위치 설정 (화면 중앙)
	SDL_QueryTexture(g_text_texture_intro, NULL, NULL, &g_text_rect_intro.w, &g_text_rect_intro.h);
	g_text_rect_intro.x = 100; // 화면 너비를 적절히 설정하세요.
	g_text_rect_intro.y = 400; // 화면 높이를 적절히 설정하세요.
	// 인트로 음악 로드
	g_music_intro = Mix_LoadMUS("../resource/introbgm.mp3");
	if (g_music_intro == NULL) {
		std::cout << "Mix_LoadMUS: " << Mix_GetError() << std::endl;
		exit(1);
	}

	// 인트로 음악 재생
	Mix_PlayMusic(g_music_intro, -1);
}


void Update_Intro()
{
	// 마우스 위치 확인
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	// 마우스와 버튼 사이의 거리 계산
	int dx = mouse_x - (g_text_rect_intro.x + g_text_rect_intro.w / 2);
	int dy = mouse_y - (g_text_rect_intro.y + g_text_rect_intro.h / 2);
	double distance = sqrt(dx * dx + dy * dy);

	// 거리에 따라 버튼 크기 변경
	if (distance < 100) {  // 마우스가 버튼 가까이에 있을 때
		g_text_rect_intro.w = 220;  // 버튼 크기를 크게
		g_text_rect_intro.h = 120;
	}
	else {  // 마우스가 버튼 멀리에 있을 때
		g_text_rect_intro.w = 200;  // 버튼 크기를 작게
		g_text_rect_intro.h = 100;
	}
}

void Render_Intro()
{
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
	SDL_RenderClear(g_renderer);
	SDL_RenderCopy(g_renderer, g_texture_intro, &g_source_rectangle_intro, &g_destination_rectangle_intro);

	// "CLICK TO START" 텍스트 렌더링
	SDL_RenderCopy(g_renderer, g_text_texture_intro, NULL, &g_text_rect_intro);
	SDL_RenderPresent(g_renderer);
}

void HandleEvents_Intro()
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
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// 마우스가 "CLICK TO START" 텍스트 위에서 클릭되었는지 확인
				if (event.button.x >= g_text_rect_intro.x &&
					event.button.x <= g_text_rect_intro.x + g_text_rect_intro.w &&
					event.button.y >= g_text_rect_intro.y &&
					event.button.y <= g_text_rect_intro.y + g_text_rect_intro.h) {
					g_current_game_phase = PHASE_STAGE1;
				}
			}

			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				g_current_game_phase = PHASE_STAGE1;
			}
			break;
		
		default:
			break;
		}
	}
}

void Clear_Intro()
{
	SDL_DestroyTexture(g_texture_intro);
	SDL_DestroyTexture(g_text_texture_intro);
	Mix_FreeMusic(g_music_intro);
	TTF_Quit();
}