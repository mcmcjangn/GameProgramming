#include "GameFunc.h"
#include <windows.h>

// 전역 변수
int g_input;

// 폰트 및 점수 출력 변수
TTF_Font* g_font;
SDL_Texture* g_score_texture;
SDL_Rect g_score_rect;
std::ostringstream score_text;
int g_score;

//전투기 위치
int g_X;
int g_Y;

//음악
Mix_Music* g_bg_mus;
Mix_Chunk* g_missile_sound;

//배경 이미지
SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;
SDL_Texture* g_bg_texture;

//전투기 이미지
SDL_Rect g_plane_source_rect;
SDL_Rect g_plane_destination_rect;
SDL_Texture* g_plane_texture;

//미사일 이미지
SDL_Rect g_missile_source_rect;
SDL_Rect g_missile_destination_rect;
SDL_Texture* g_missile_texture;

// 미사일 위치를 저장하는 구조체
struct Missile {
	int x;
	int y;
};

// 미사일 최대 개수
const int MAX_MISSILES = 3;

// 미사일 배열
Missile g_missiles[MAX_MISSILES];

// 현재 미사일 개수
int g_missile_count = 0;


bool g_key_left = false;
bool g_key_right = false;
bool g_key_up = false;
bool g_key_down = false;
bool g_key_space = false;

// 흘러간 시간 기록
double g_elapsed_time_ms;

//미사일 발사 여부
bool g_flag_missile;

//미사일 위치
int g_missile_X;
int g_missile_Y;
std::string g_missile_output;

/////////////////////////////////////////////////////////////
// InitGame() 
// 프로그램이 시작될 때 최초에 한 번 호출되는 함수.
// 이 함수에서 게임에 필요한 자원(이미지, 사운드 등)을 로딩하고, 상태 변수들을 초기화 해야한다.
void InitGame() {
	g_flag_running = true;

	//BG
	SDL_Surface* bg_surface = IMG_Load("../resource/star.jpg");
	g_bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	//SDL_Texture* bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);

	g_bg_source_rect.x = 300;
	g_bg_source_rect.y = 200;
	g_bg_source_rect.w = 700;
	g_bg_source_rect.h = 800;

	g_bg_destination_rect.x = 0;
	g_bg_destination_rect.y = 0;
	g_bg_destination_rect.w = 400;
	g_bg_destination_rect.h = 600;

	//Plane
	SDL_Surface* plane_surface = IMG_Load("../resource/plane.png");
	g_plane_texture = SDL_CreateTextureFromSurface(g_renderer, plane_surface);
	SDL_FreeSurface(plane_surface);

	g_plane_source_rect.x = 0;
	g_plane_source_rect.y = 0;
	g_plane_source_rect.w = 100;
	g_plane_source_rect.h = 100;

	g_plane_destination_rect.x = 150;
	g_plane_destination_rect.y = 500;
	g_plane_destination_rect.w = 100;
	g_plane_destination_rect.h = 100;

	//Missile
	SDL_Surface* missile_surface = IMG_Load("../resource/missile.png");
	g_missile_texture = SDL_CreateTextureFromSurface(g_renderer, missile_surface);
	SDL_FreeSurface(missile_surface);

	g_missile_source_rect.x = 0;
	g_missile_source_rect.y = 0;
	g_missile_source_rect.w = 100;
	g_missile_source_rect.h = 100;

	g_missile_destination_rect.x = 0;
	g_missile_destination_rect.y = 0;
	g_missile_destination_rect.w = 100;
	g_missile_destination_rect.h = 100;

	//전투기 위치 초기화
	g_X = 150;
	g_Y = 400;

	// 폰트 초기화
	TTF_Init();
	g_font = TTF_OpenFont("../resource/Mor.ttf", 38);
	if (g_font == NULL) {
		std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
		exit(1);
	}
	// 점수 초기화

	//배경음악 로드
	g_bg_mus = Mix_LoadMUS("../resource/bgbgm.mp3");
	if (g_bg_mus == NULL) {
		std::cout << "Mix_LoadMUS: " << Mix_GetError() << std::endl;
		exit(1);
	}

	//미사일 효과음 로드
	g_missile_sound = Mix_LoadWAV("../resource/hit.wav");

	//배경음 재생
	if (Mix_PlayMusic(g_bg_mus, -1) == -1) {
		std::cout << "Mix_PlayMusic: " << Mix_GetError() << std::endl;
	}

	//음악볼륨 조절
	Mix_VolumeMusic(100);
}






/////////////////////////////////////////////////////////////
// Update() 
// 게임의 내용을 업데이트하는 함수.
// 실제 게임의 룰을 구현해야하는 곳.
// 게임에서 일어나는 변화는 모두 이 곳에서 구현한다.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Update() {
	// 좌측 이동
	if (g_key_left) {
		g_X -= 10;
		if (g_X < 0) {
			g_X = 0;
		}
	}
	// 우측 이동
	else if (g_key_right) {
		g_X += 10;
		if (g_X > SCREEN_WIDTH - g_plane_destination_rect.w) {
			g_X = SCREEN_WIDTH - g_plane_destination_rect.w;
		}
	}
	// 위로 이동
	else if (g_key_up) {
		g_Y -= 10;
		if (g_Y < 0) {
			g_Y = 0;
		}
	}
	// 아래로 이동
	else if (g_key_down) {
		g_Y += 10;
		if (g_Y > SCREEN_HEIGHT - g_plane_destination_rect.h) {
			g_Y = SCREEN_HEIGHT - g_plane_destination_rect.h;
		}
	}
	// 미사일 추가
	if (g_key_space) {
		// 미사일이 MAX_MISSILES 미만일 때만 추가
		if (g_missile_count < MAX_MISSILES) {
			Mix_PlayChannel(-1, g_missile_sound, 0);
			g_missiles[g_missile_count].x = g_X;
			g_missiles[g_missile_count].y = g_Y;
			g_missile_count++;
			g_score += 10;  // 점수 증가
		}
		g_key_space = false;  // 미사일 발사 후 키 상태를 false로 변경
	}

	// 점수 텍스처 업데이트
	std::ostringstream score_text;
	score_text << "Score: " << std::setw(5) << std::setfill('0') << g_score;

	//경계선이 불투명하게 자연스럽게 출력
	SDL_Surface* score_surface = TTF_RenderText_Solid(g_font, score_text.str().c_str(), { 255, 255, 0 });
	g_score_texture = SDL_CreateTextureFromSurface(g_renderer, score_surface);
	SDL_FreeSurface(score_surface);
	SDL_QueryTexture(g_score_texture, NULL, NULL, &g_score_rect.w, &g_score_rect.h);
	g_score_rect.x = SCREEN_WIDTH - 110;
	g_score_rect.y = SCREEN_HEIGHT - 40;

	g_elapsed_time_ms += 33;
}




/////////////////////////////////////////////////////////////
// Render() 
// 그림을 그리는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Render() {
	//// 1. 배경 그리기.
	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);

	// 2. 전투기 그리기.
	g_plane_destination_rect.x = g_X;
	g_plane_destination_rect.y = g_Y;
	SDL_RenderCopy(g_renderer, g_plane_texture, &g_plane_source_rect, &g_plane_destination_rect);

	// 화면 밖으로 나간 미사일 제거
	for (int i = 0; i < g_missile_count; i++) {
		if (g_missiles[i].y < 0) {
			// 배열의 마지막 미사일을 현재 위치로 이동
			g_missiles[i] = g_missiles[g_missile_count - 1];
			g_missile_count--;
			i--;  // 다시 현재 위치의 미사일을 확인
		}
	}
	// 3. 미사일 그리기.
	for (int i = 0; i < g_missile_count; i++) {
		g_missile_destination_rect.x = g_missiles[i].x;
		g_missile_destination_rect.y = g_missiles[i].y - 70;
		g_missiles[i].y -= 10;
		SDL_RenderCopy(g_renderer, g_missile_texture, &g_missile_source_rect, &g_missile_destination_rect);
	}
	//글씨 출력
	SDL_RenderCopy(g_renderer, g_score_texture, NULL, &g_score_rect);

	SDL_RenderPresent(g_renderer);
}


////////////////////////////////////////////////////////////
// HandleEvents() 
// 이벤트를 처리하는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_LEFT) {
				g_key_left = true;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_key_right = true;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {
				g_key_space = true;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				g_key_up = true;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				g_key_down = true;
			}
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LEFT) {
				g_key_left = false;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_key_right = false;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {
				g_key_space = false;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				g_key_up = false;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				g_key_down = false;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:

			// 마우스 왼쪽 버튼이 눌려 졌을 때.
			if (event.button.button == SDL_BUTTON_LEFT) {
				g_input = 0;
			}
			break;

		default:
			break;
		}
	}
}



/////////////////////////////////////////////////////////////
// ClearGame() 
// 프로그램이 끝날 때 한 번 호출되는 함수.
// 이 함수에서 사용된 자원(이미지, 사운드 등)과 메모리 등을 해제해야한다.
void ClearGame()
{
	SDL_DestroyTexture(g_bg_texture);
	SDL_DestroyTexture(g_plane_texture);
	SDL_DestroyTexture(g_missile_texture);

	Mix_FreeMusic(g_bg_mus);
	Mix_FreeChunk(g_missile_sound);

	TTF_CloseFont(g_font);
	SDL_DestroyTexture(g_score_texture);
}
