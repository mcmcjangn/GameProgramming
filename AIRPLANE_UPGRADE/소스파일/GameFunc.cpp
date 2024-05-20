#include "GameFunc.h"
#include <windows.h>

// ���� ����
int g_input;

// ��Ʈ �� ���� ��� ����
TTF_Font* g_font;
SDL_Texture* g_score_texture;
SDL_Rect g_score_rect;
std::ostringstream score_text;
int g_score;

//������ ��ġ
int g_X;
int g_Y;

//����
Mix_Music* g_bg_mus;
Mix_Chunk* g_missile_sound;

//��� �̹���
SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;
SDL_Texture* g_bg_texture;

//������ �̹���
SDL_Rect g_plane_source_rect;
SDL_Rect g_plane_destination_rect;
SDL_Texture* g_plane_texture;

//�̻��� �̹���
SDL_Rect g_missile_source_rect;
SDL_Rect g_missile_destination_rect;
SDL_Texture* g_missile_texture;

// �̻��� ��ġ�� �����ϴ� ����ü
struct Missile {
	int x;
	int y;
};

// �̻��� �ִ� ����
const int MAX_MISSILES = 3;

// �̻��� �迭
Missile g_missiles[MAX_MISSILES];

// ���� �̻��� ����
int g_missile_count = 0;


bool g_key_left = false;
bool g_key_right = false;
bool g_key_up = false;
bool g_key_down = false;
bool g_key_space = false;

// �귯�� �ð� ���
double g_elapsed_time_ms;

//�̻��� �߻� ����
bool g_flag_missile;

//�̻��� ��ġ
int g_missile_X;
int g_missile_Y;
std::string g_missile_output;

/////////////////////////////////////////////////////////////
// InitGame() 
// ���α׷��� ���۵� �� ���ʿ� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���ӿ� �ʿ��� �ڿ�(�̹���, ���� ��)�� �ε��ϰ�, ���� �������� �ʱ�ȭ �ؾ��Ѵ�.
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

	//������ ��ġ �ʱ�ȭ
	g_X = 150;
	g_Y = 400;

	// ��Ʈ �ʱ�ȭ
	TTF_Init();
	g_font = TTF_OpenFont("../resource/Mor.ttf", 38);
	if (g_font == NULL) {
		std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
		exit(1);
	}
	// ���� �ʱ�ȭ

	//������� �ε�
	g_bg_mus = Mix_LoadMUS("../resource/bgbgm.mp3");
	if (g_bg_mus == NULL) {
		std::cout << "Mix_LoadMUS: " << Mix_GetError() << std::endl;
		exit(1);
	}

	//�̻��� ȿ���� �ε�
	g_missile_sound = Mix_LoadWAV("../resource/hit.wav");

	//����� ���
	if (Mix_PlayMusic(g_bg_mus, -1) == -1) {
		std::cout << "Mix_PlayMusic: " << Mix_GetError() << std::endl;
	}

	//���Ǻ��� ����
	Mix_VolumeMusic(100);
}






/////////////////////////////////////////////////////////////
// Update() 
// ������ ������ ������Ʈ�ϴ� �Լ�.
// ���� ������ ���� �����ؾ��ϴ� ��.
// ���ӿ��� �Ͼ�� ��ȭ�� ��� �� ������ �����Ѵ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Update() {
	// ���� �̵�
	if (g_key_left) {
		g_X -= 10;
		if (g_X < 0) {
			g_X = 0;
		}
	}
	// ���� �̵�
	else if (g_key_right) {
		g_X += 10;
		if (g_X > SCREEN_WIDTH - g_plane_destination_rect.w) {
			g_X = SCREEN_WIDTH - g_plane_destination_rect.w;
		}
	}
	// ���� �̵�
	else if (g_key_up) {
		g_Y -= 10;
		if (g_Y < 0) {
			g_Y = 0;
		}
	}
	// �Ʒ��� �̵�
	else if (g_key_down) {
		g_Y += 10;
		if (g_Y > SCREEN_HEIGHT - g_plane_destination_rect.h) {
			g_Y = SCREEN_HEIGHT - g_plane_destination_rect.h;
		}
	}
	// �̻��� �߰�
	if (g_key_space) {
		// �̻����� MAX_MISSILES �̸��� ���� �߰�
		if (g_missile_count < MAX_MISSILES) {
			Mix_PlayChannel(-1, g_missile_sound, 0);
			g_missiles[g_missile_count].x = g_X;
			g_missiles[g_missile_count].y = g_Y;
			g_missile_count++;
			g_score += 10;  // ���� ����
		}
		g_key_space = false;  // �̻��� �߻� �� Ű ���¸� false�� ����
	}

	// ���� �ؽ�ó ������Ʈ
	std::ostringstream score_text;
	score_text << "Score: " << std::setw(5) << std::setfill('0') << g_score;

	//��輱�� �������ϰ� �ڿ������� ���
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
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Render() {
	//// 1. ��� �׸���.
	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);

	// 2. ������ �׸���.
	g_plane_destination_rect.x = g_X;
	g_plane_destination_rect.y = g_Y;
	SDL_RenderCopy(g_renderer, g_plane_texture, &g_plane_source_rect, &g_plane_destination_rect);

	// ȭ�� ������ ���� �̻��� ����
	for (int i = 0; i < g_missile_count; i++) {
		if (g_missiles[i].y < 0) {
			// �迭�� ������ �̻����� ���� ��ġ�� �̵�
			g_missiles[i] = g_missiles[g_missile_count - 1];
			g_missile_count--;
			i--;  // �ٽ� ���� ��ġ�� �̻����� Ȯ��
		}
	}
	// 3. �̻��� �׸���.
	for (int i = 0; i < g_missile_count; i++) {
		g_missile_destination_rect.x = g_missiles[i].x;
		g_missile_destination_rect.y = g_missiles[i].y - 70;
		g_missiles[i].y -= 10;
		SDL_RenderCopy(g_renderer, g_missile_texture, &g_missile_source_rect, &g_missile_destination_rect);
	}
	//�۾� ���
	SDL_RenderCopy(g_renderer, g_score_texture, NULL, &g_score_rect);

	SDL_RenderPresent(g_renderer);
}


////////////////////////////////////////////////////////////
// HandleEvents() 
// �̺�Ʈ�� ó���ϴ� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
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

			// ���콺 ���� ��ư�� ���� ���� ��.
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
// ���α׷��� ���� �� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���� �ڿ�(�̹���, ���� ��)�� �޸� ���� �����ؾ��Ѵ�.
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
