#include "GameFunc.h"
#include "GameFuncStage1.h"

// ���� ����
int g_input;

// ��Ʈ �� ���� ��� ����
TTF_Font* g_font;
TTF_Font* g_font_reload;
SDL_Texture* g_score_texture;
SDL_Rect g_score_rect;
std::ostringstream score_text;
int g_score;

//������ ��ġ
int g_X;
int g_Y;

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// ������� ���� ����
Direction g_direction = UP;

//����
Mix_Chunk* g_missile_sound;
Mix_Chunk* g_reload_sound;
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
	Direction direction;
};

// �̻��� �ִ� ����
const int MAX_MISSILES = 5;

// �̻��� �迭
Missile g_missiles[MAX_MISSILES];

// ���� �̻��� ����
int g_missile_count = 0;

// ������ ��ư �̹���
SDL_Rect g_reload_button_source_rect;
SDL_Rect g_reload_button_destination_rect;
SDL_Texture* g_reload_button_texture;

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

static SDL_Texture* g_reload_text_texture; // the SDL_Texture for text
static SDL_Rect g_reload_text_rect; // the rectangle for text

/////////////////////////////////////////////////////////////
// InitGame() 
// ���α׷��� ���۵� �� ���ʿ� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���ӿ� �ʿ��� �ڿ�(�̹���, ���� ��)�� �ε��ϰ�, ���� �������� �ʱ�ȭ �ؾ��Ѵ�.
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

	g_font_reload = TTF_OpenFont("../resource/hel.ttf", 10);
	if (g_font == NULL) {
		std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
		exit(1);
	}

	//�̻��� ȿ���� �ε�
	g_missile_sound = Mix_LoadWAV("../resource/hit.wav");
	//������ ȿ���� �ε�
	g_reload_sound = Mix_LoadWAV("../resource/reload.wav");
	if (g_reload_sound == NULL) {
		std::cout << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
		exit(1);
	}


	//���Ǻ��� ����
	Mix_VolumeMusic(100);
	// ������ ��ư �̹��� �ε�
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


	// "������" �ؽ�Ʈ �ؽ�ó ����
	SDL_Surface* reload_text_surface = TTF_RenderText_Solid(g_font_reload, "RELOAD", { 0, 0, 0 });
	g_reload_text_texture = SDL_CreateTextureFromSurface(g_renderer, reload_text_surface);
	SDL_FreeSurface(reload_text_surface);

	// �ؽ�Ʈ ��ġ ���� (��ư ��)
	SDL_QueryTexture(g_reload_text_texture, NULL, NULL, &g_reload_text_rect.w, &g_reload_text_rect.h);
	g_reload_text_rect.x = g_reload_button_destination_rect.x + 13;
	g_reload_text_rect.y = g_reload_button_destination_rect.y + 25;

}






/////////////////////////////////////////////////////////////
// Update() 
// ������ ������ ������Ʈ�ϴ� �Լ�.
// ���� ������ ���� �����ؾ��ϴ� ��.
// ���ӿ��� �Ͼ�� ��ȭ�� ��� �� ������ �����Ѵ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Update_Stage1() {
	// ���� �̵�
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

	// ���� �̵�
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


	// �̻��� �߰�
	if (g_key_space) {
		// �̻����� 5�� �̸��� ���� �߰�
		if (g_missile_count < 5) {
			Mix_PlayChannel(-1, g_missile_sound, 0);
			switch (g_direction) {
			case RIGHT:
				g_missiles[g_missile_count].x = g_X + 50;  // ������� �Ӹ� �κ�
				g_missiles[g_missile_count].y = g_Y;  // ������� �߾�
				break;
			case LEFT:
				g_missiles[g_missile_count].x = g_X - 50;  // ������� �Ӹ� �κ�
				g_missiles[g_missile_count].y = g_Y;  // ������� �߾�
				break;
			case UP:
				g_missiles[g_missile_count].x = g_X;  // ������� �߾�
				g_missiles[g_missile_count].y = g_Y - 50;  // ������� �Ӹ� �κ�
				break;
			case DOWN:
				g_missiles[g_missile_count].x = g_X;  // ������� �߾�
				g_missiles[g_missile_count].y = g_Y + 50;  // ������� �Ӹ� �κ�
				break;
			}
			g_missiles[g_missile_count].direction = g_direction;  // �̻����� �̵� ���� ����
			g_missile_count++;
			g_score += 10;  // ���� ����
		}

		g_key_space = false;  // �̻��� �߻� �� Ű ���¸� false�� ����

	}

	// �̻��� �̵�
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

	//���� �̻��� ���� ���
	std::string missile_count_str = std::to_string(5 - g_missile_count);

	//��輱�� �������ϰ� �ڿ������� ���
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
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Render_Stage1() {
	//// 1. ��� �׸���.
	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);

	// 2. ������ �׸���.
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

	// ȭ�� ������ ���� �̻��� ����
	for (int i = 0; i < g_missile_count; i++) {
		// �̻����� ȭ�� �������� �������� Ȯ��
		if (g_missiles[i].y < -70) {
			g_missiles[i] = g_missiles[g_missile_count - 1];
		}
		// �̻����� ȭ�� �Ʒ������� �������� Ȯ��
		else if (g_missiles[i].y > SCREEN_HEIGHT + 70) {
			g_missiles[i] = g_missiles[g_missile_count - 1];
		}
		// �̻����� ȭ�� �������� �������� Ȯ��
		else if (g_missiles[i].x < -70) {
			g_missiles[i] = g_missiles[g_missile_count - 1];
		}
		// �̻����� ȭ�� ���������� �������� Ȯ��
		else if (g_missiles[i].x > SCREEN_WIDTH) {
			g_missiles[i] = g_missiles[g_missile_count - 1];
		}
	}
	// 3. �̻��� �׸���.
	
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
	//�۾� ���
	SDL_RenderCopy(g_renderer, g_score_texture, NULL, &g_score_rect);
	//������ ��ư
	if (g_missile_count == 5) { // �̻��� ������ 0�� ������ ��ư�� �׸��ϴ�.
		SDL_RenderCopy(g_renderer, g_reload_button_texture, &g_reload_button_source_rect, &g_reload_button_destination_rect);

		// "������" �ؽ�Ʈ �׸���
		SDL_RenderCopy(g_renderer, g_reload_text_texture, NULL, &g_reload_text_rect);
	}
	SDL_RenderPresent(g_renderer);
}


////////////////////////////////////////////////////////////
// HandleEvents() 
// �̺�Ʈ�� ó���ϴ� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
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

			// ���콺 ���� ��ư�� ���� ���� ��.
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (g_missile_count == 5) {
					// ���콺�� ������ ��ư ������ Ŭ���Ǿ����� Ȯ��
					if (event.button.x >= g_reload_button_destination_rect.x &&
						event.button.x <= g_reload_button_destination_rect.x + g_reload_button_destination_rect.w &&
						event.button.y >= g_reload_button_destination_rect.y &&
						event.button.y <= g_reload_button_destination_rect.y + g_reload_button_destination_rect.h) {
						// �̻��� ������ �ٽ� 5���� ����
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
// ���α׷��� ���� �� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���� �ڿ�(�̹���, ���� ��)�� �޸� ���� �����ؾ��Ѵ�.
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
