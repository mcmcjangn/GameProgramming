#include "GameFunc.h"
#include "GameFuncStage1.h"

// 전역 변수
int g_input;

// 폰트 및 점수 출력 변수
TTF_Font* g_font;
TTF_Font* g_font_reload;
SDL_Texture* g_score_texture;
SDL_Rect g_score_rect;
std::ostringstream score_text;
int g_score;

//전투기 위치
int g_X;
int g_Y;

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// 비행기의 현재 방향
Direction g_direction = UP;

//음악
Mix_Chunk* g_missile_sound;
Mix_Chunk* g_reload_sound;
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
	Direction direction;
};

// 미사일 최대 개수
const int MAX_MISSILES = 5;

// 미사일 배열
Missile g_missiles[MAX_MISSILES];

// 현재 미사일 개수
int g_missile_count = 0;

// 재장전 버튼 이미지
SDL_Rect g_reload_button_source_rect;
SDL_Rect g_reload_button_destination_rect;
SDL_Texture* g_reload_button_texture;

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

static SDL_Texture* g_reload_text_texture; // the SDL_Texture for text
static SDL_Rect g_reload_text_rect; // the rectangle for text

/////////////////////////////////////////////////////////////
// InitGame() 
// 프로그램이 시작될 때 최초에 한 번 호출되는 함수.
// 이 함수에서 게임에 필요한 자원(이미지, 사운드 등)을 로딩하고, 상태 변수들을 초기화 해야한다.
void Init_Stage1() {
	Mix_HaltMusic();
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
	g_direction = UP;
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

	g_font_reload = TTF_OpenFont("../resource/hel.ttf", 10);
	if (g_font == NULL) {
		std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
		exit(1);
	}

	//미사일 효과음 로드
	g_missile_sound = Mix_LoadWAV("../resource/hit.wav");
	//재장전 효과음 로드
	g_reload_sound = Mix_LoadWAV("../resource/reload.wav");
	if (g_reload_sound == NULL) {
		std::cout << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
		exit(1);
	}


	//음악볼륨 조절
	Mix_VolumeMusic(100);
	// 재장전 버튼 이미지 로드
	SDL_Surface* reload_button_surface = IMG_Load("../resource/button.png");
	g_reload_button_texture = SDL_CreateTextureFromSurface(g_renderer, reload_button_surface);
	SDL_FreeSurface(reload_button_surface);

	g_reload_button_source_rect.x = 0;
	g_reload_button_source_rect.y = 0;
	g_reload_button_source_rect.w = 600;
	g_reload_button_source_rect.h = 560;

	g_reload_button_destination_rect.x = 300;
	g_reload_button_destination_rect.y = 540;
	g_reload_button_destination_rect.w = 60;
	g_reload_button_destination_rect.h = 60;


	// "재장전" 텍스트 텍스처 생성
	SDL_Surface* reload_text_surface = TTF_RenderText_Solid(g_font_reload, "RELOAD", { 0, 0, 0 });
	g_reload_text_texture = SDL_CreateTextureFromSurface(g_renderer, reload_text_surface);
	SDL_FreeSurface(reload_text_surface);

	// 텍스트 위치 설정 (버튼 위)
	SDL_QueryTexture(g_reload_text_texture, NULL, NULL, &g_reload_text_rect.w, &g_reload_text_rect.h);
	g_reload_text_rect.x = g_reload_button_destination_rect.x + 13;
	g_reload_text_rect.y = g_reload_button_destination_rect.y + 25;

}






/////////////////////////////////////////////////////////////
// Update() 
// 게임의 내용을 업데이트하는 함수.
// 실제 게임의 룰을 구현해야하는 곳.
// 게임에서 일어나는 변화는 모두 이 곳에서 구현한다.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Update_Stage1() {
	// 수평 이동
	if (g_key_left) {
		g_X -= 10;
		if (g_X < 0) {
			g_X = 0;
		}
		g_direction = LEFT;
	}
	if (g_key_right) {
		g_X += 10;
		if (g_X > SCREEN_WIDTH - g_plane_destination_rect.w) {
			g_X = SCREEN_WIDTH - g_plane_destination_rect.w;
		}
		g_direction = RIGHT;
	}

	// 수직 이동
	if (g_key_up) {
		g_Y -= 10;
		if (g_Y < 0) {
			g_Y = 0;
		}
		g_direction = UP;
	}
	if (g_key_down) {
		g_Y += 10;
		if (g_Y > SCREEN_HEIGHT - g_plane_destination_rect.h) {
			g_Y = SCREEN_HEIGHT - g_plane_destination_rect.h;
		}
		g_direction = DOWN;
	}


	// 미사일 추가
	if (g_key_space) {
		// 미사일이 5개 미만일 때만 추가
		if (g_missile_count < 5) {
			Mix_PlayChannel(-1, g_missile_sound, 0);
			switch (g_direction) {
			case RIGHT:
				g_missiles[g_missile_count].x = g_X + 50;  // 비행기의 머리 부분
				g_missiles[g_missile_count].y = g_Y;  // 비행기의 중앙
				break;
			case LEFT:
				g_missiles[g_missile_count].x = g_X - 50;  // 비행기의 머리 부분
				g_missiles[g_missile_count].y = g_Y;  // 비행기의 중앙
				break;
			case UP:
				g_missiles[g_missile_count].x = g_X;  // 비행기의 중앙
				g_missiles[g_missile_count].y = g_Y - 50;  // 비행기의 머리 부분
				break;
			case DOWN:
				g_missiles[g_missile_count].x = g_X;  // 비행기의 중앙
				g_missiles[g_missile_count].y = g_Y + 50;  // 비행기의 머리 부분
				break;
			}
			g_missiles[g_missile_count].direction = g_direction;  // 미사일의 이동 방향 설정
			g_missile_count++;
			g_score += 10;  // 점수 증가
		}

		g_key_space = false;  // 미사일 발사 후 키 상태를 false로 변경

	}

	// 미사일 이동
	for (int i = 0; i < g_missile_count; i++) {
		switch (g_missiles[i].direction) {
		case RIGHT:
			g_missiles[i].x += 20;
			break;
		case LEFT:
			g_missiles[i].x -= 20;
			break;
		case UP:
			g_missiles[i].y -= 20;
			break;
		case DOWN:
			g_missiles[i].y += 20;
			break;
		}
	}

	//현재 미사일 개수 출력
	std::string missile_count_str = std::to_string(5 - g_missile_count);

	//경계선이 불투명하게 자연스럽게 출력
	SDL_Surface* score_surface = TTF_RenderText_Solid(g_font, missile_count_str.c_str(), { 255, 255, 0 });
	g_score_texture = SDL_CreateTextureFromSurface(g_renderer, score_surface);
	SDL_FreeSurface(score_surface);
	SDL_QueryTexture(g_score_texture, NULL, NULL, &g_score_rect.w, &g_score_rect.h);
	g_score_rect.x = SCREEN_WIDTH - 30;
	g_score_rect.y = SCREEN_HEIGHT - 40;

	g_elapsed_time_ms += 33;
}




/////////////////////////////////////////////////////////////
// Render() 
// 그림을 그리는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Render_Stage1() {
	//// 1. 배경 그리기.
	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);

	// 2. 전투기 그리기.
	g_plane_destination_rect.x = g_X;
	g_plane_destination_rect.y = g_Y;
	double angle;
	switch (g_direction) {
	case RIGHT:
		angle = 90.0;
		break;
	case LEFT:
		angle = -90.0;
		break;
	case DOWN:
		angle = 180.0;
		break;
	default:
		angle = 0.0;
		break;
	}
	SDL_Point center = { g_plane_destination_rect.w / 2, g_plane_destination_rect.h / 2 };
	SDL_RenderCopyEx(g_renderer, g_plane_texture, &g_plane_source_rect, &g_plane_destination_rect, angle, &center, SDL_FLIP_NONE);

	// 화면 밖으로 나간 미사일 제거
	for (int i = 0; i < g_missile_count; i++) {
		// 미사일이 화면 위쪽으로 나갔는지 확인
		if (g_missiles[i].y < -70) {
			g_missiles[i] = g_missiles[g_missile_count - 1];
		}
		// 미사일이 화면 아래쪽으로 나갔는지 확인
		else if (g_missiles[i].y > SCREEN_HEIGHT + 70) {
			g_missiles[i] = g_missiles[g_missile_count - 1];
		}
		// 미사일이 화면 왼쪽으로 나갔는지 확인
		else if (g_missiles[i].x < -70) {
			g_missiles[i] = g_missiles[g_missile_count - 1];
		}
		// 미사일이 화면 오른쪽으로 나갔는지 확인
		else if (g_missiles[i].x > SCREEN_WIDTH) {
			g_missiles[i] = g_missiles[g_missile_count - 1];
		}
	}
	// 3. 미사일 그리기.
	
	for (int i = 0; i < g_missile_count; i++) {
		g_missile_destination_rect.x = g_missiles[i].x;
		g_missile_destination_rect.y = g_missiles[i].y;
		g_missile_destination_rect.w = 100;
		g_missile_destination_rect.h = 100;
		switch (g_missiles[i].direction) {
		case RIGHT:
			angle = 90.0;
			break;
		case LEFT:
			angle = -90.0;
			break;
		case DOWN:
			angle = 180.0;
			break;
		default:
			angle = 0.0;
			break;
		}
		SDL_Point center = { g_missile_destination_rect.w/2, g_missile_destination_rect.h/2 };
		SDL_RenderCopyEx(g_renderer, g_missile_texture, NULL, &g_missile_destination_rect, angle, &center, SDL_FLIP_NONE);

		//SDL_RenderCopy(g_renderer, g_missile_texture, &g_missile_source_rect, &g_missile_destination_rect);
	}
	//글씨 출력
	SDL_RenderCopy(g_renderer, g_score_texture, NULL, &g_score_rect);
	//재장전 버튼
	if (g_missile_count == 5) { // 미사일 개수가 0일 때에만 버튼을 그립니다.
		SDL_RenderCopy(g_renderer, g_reload_button_texture, &g_reload_button_source_rect, &g_reload_button_destination_rect);

		// "재장전" 텍스트 그리기
		SDL_RenderCopy(g_renderer, g_reload_text_texture, NULL, &g_reload_text_rect);
	}
	SDL_RenderPresent(g_renderer);
}


////////////////////////////////////////////////////////////
// HandleEvents() 
// 이벤트를 처리하는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void HandleEvents_Stage1()
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			// Set the direction based on the newly pressed key
			if (event.key.keysym.sym == SDLK_LEFT) {
				g_key_left = true;
				g_key_right = false;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_key_right = true;
				g_key_left = false;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				g_key_up = true;
				g_key_down = false;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				g_key_down = true;
				g_key_up = false;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {
				g_key_space = true;
			}
			break;

		case SDL_KEYUP:
			// Clear the direction when the corresponding key is released
			if (event.key.keysym.sym == SDLK_LEFT) {
				g_key_left = false;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_key_right = false;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				g_key_up = false;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				g_key_down = false;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {
				g_key_space = false;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:

			// 마우스 왼쪽 버튼이 눌려 졌을 때.
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (g_missile_count == 5) {
					// 마우스가 재장전 버튼 위에서 클릭되었는지 확인
					if (event.button.x >= g_reload_button_destination_rect.x &&
						event.button.x <= g_reload_button_destination_rect.x + g_reload_button_destination_rect.w &&
						event.button.y >= g_reload_button_destination_rect.y &&
						event.button.y <= g_reload_button_destination_rect.y + g_reload_button_destination_rect.h) {
						// 미사일 개수를 다시 5개로 설정
						g_missile_count = 0;
						Mix_PlayChannel(-1, g_reload_sound, 0);

					}
				}
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				g_current_game_phase = PHASE_ENDING;
				g_missile_count = 0;
				g_score = 0;
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
void Clear_Stage1()
{
	SDL_DestroyTexture(g_bg_texture);
	SDL_DestroyTexture(g_plane_texture);
	SDL_DestroyTexture(g_missile_texture);
	SDL_DestroyTexture(g_reload_button_texture);
	SDL_DestroyTexture(g_reload_text_texture);

	Mix_FreeChunk(g_missile_sound);
	Mix_FreeChunk(g_reload_sound);
	TTF_CloseFont(g_font);
	TTF_CloseFont(g_font_reload);	
	SDL_DestroyTexture(g_score_texture);

}
