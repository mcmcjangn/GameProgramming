#include "GameFunc.h"
#include "snake.h"
#include <windows.h>
#include <queue>
#include "SDL.h"
#include <atlstr.h>


Snake::Snake()
{

	// For Character
	ch_x = 400, ch_y = 300;
	//speed = 1;
	//�������
	music = Mix_LoadMUS("../Resources/bgm.mp3");
	if (music == NULL) {
		std::cout << "Mix_LoadMUS: " << Mix_GetError() << std::endl;
		exit(1);
	}
	//���� �Դ� �Ҹ�
	eat_sound = Mix_LoadWAV("../Resources/eat.wav");
	if (eat_sound == NULL) {
		std::cout << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
		exit(1);
	}

	//�״� �Ҹ�
	die_sound = Mix_LoadWAV("../Resources/dead.wav");
	if (die_sound == NULL) {
		std::cout << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
		exit(1);
	}

	//bg
	SDL_Surface* bg_surface = IMG_Load("../Resources/bg.jpg");
	bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);
	bg_source_rect = { 0, 0, 2000, 2000 };
	bg_dest_rect = { 0, 0, 800, 600 };

	// ��������Ʈ ��Ʈ 
	SDL_Surface* food_sheet_surface = IMG_Load("../Resources/seaAnimals/seaCreature.png");
	SDL_Texture* food_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, food_sheet_surface);
	SDL_FreeSurface(food_sheet_surface);

	//food_texturesv ���Ϳ� �߰�
	food_texturesv.clear();
	food_rects.clear();

	for (int i = 0; i < numSpritesY; i++) {
		for (int j = 0; j < numSpritesX; j++) {
			SDL_Rect spriteRect = { j * spriteWidth, i * spriteHeight, spriteWidth, spriteHeight };
			food_texturesv.push_back(food_sheet_texture);
			food_rects.push_back(spriteRect);
		}
	}

	//snake 
	SDL_Surface* snake_surface = IMG_Load("../Resources/Snake.png");
	snake_texture = SDL_CreateTextureFromSurface(g_renderer, snake_surface);
	SDL_FreeSurface(snake_surface);
	snake_rect = { 0, 0, 402, 402 };
	snake.push_front({ ch_x, ch_y, WIDTH, HEIGHT });

	// snake.cpp ���Ͽ��� ���� �Ӹ� �ؽ�ó�� �ε��մϴ�.
	SDL_Surface* snake_head_surface = IMG_Load("../Resources/snake_head.png");
	snake_head_texture = SDL_CreateTextureFromSurface(g_renderer, snake_head_surface);
	SDL_FreeSurface(snake_head_surface);
	// snake.cpp ���Ͽ��� ���� ���� �ؽ�ó�� �ε��մϴ�.
	SDL_Surface* snake_body_surface = IMG_Load("../Resources/snake_body.png");
	snake_body_texture = SDL_CreateTextureFromSurface(g_renderer, snake_body_surface);
	SDL_FreeSurface(snake_body_surface);
	// snake.cpp ���Ͽ��� ���� ���� �ؽ�ó�� �ε��մϴ�.
	SDL_Surface* snake_tail_surface = IMG_Load("../Resources/snake_tail.png");
	snake_tail_texture = SDL_CreateTextureFromSurface(g_renderer, snake_tail_surface);
	SDL_FreeSurface(snake_tail_surface);

	//Ready
	SDL_Surface* ready_surface = IMG_Load("../Resources/Ready.png");
	SDL_SetSurfaceAlphaMod(ready_surface, 200); // ���� ���� ���� (0~255 ����, 0�� ���� ����, 255�� ���� ������)
	ready_texture = SDL_CreateTextureFromSurface(g_renderer, ready_surface);
	SDL_FreeSurface(ready_surface);
	ready_rect = { 0, 0, 560, 560 };
	ready_dest = { 200, 160, 400, 320 };

	//Gameover
	SDL_Surface* Gameover_surface = IMG_Load("../Resources/Gameover.png");
	Gameover_texture = SDL_CreateTextureFromSurface(g_renderer, Gameover_surface);
	SDL_FreeSurface(Gameover_surface);
	Gameover_rect = { 0, 0, 3508, 2480 };
	Gameover_dest = { 150, 70, 500,500 };

	//score ��Ʈ
	font = TTF_OpenFont("../Resources/pxfont.ttf", 24);
	color = { 0, 98, 98 };
	// �ؽ�Ʈ�� ��ġ�� ũ�� ����
	text_rect.x = 10;
	text_rect.y = 10;
	text_rect.w = 100;
	text_rect.h = 50;

	food = false;
	GenerateFood();
}

void Snake::GenerateFood()
{
	// ������ ��� ��ġ�� ã���ϴ�.
	std::vector<SDL_Rect> possible_positions;
	for (int x = 0; x < SCREEN_WIDTH; x += GRID) {
		for (int y = 0; y < SCREEN_HEIGHT; y += GRID) {
			possible_positions.push_back({ x, y, GRID, GRID });
		}
	}

	// ���� ��ġ�� �����մϴ�.
	for (const auto& part : snake) {
		possible_positions.erase(std::remove_if(possible_positions.begin(), possible_positions.end(),
			[&part](const SDL_Rect& rect) {
				return rect.x == part.x && rect.y == part.y;
			}), possible_positions.end());
	}

	// ������ ��ġ �߿��� �����ϰ� �����մϴ�.
	if (!possible_positions.empty()) {
		int index = rand() % possible_positions.size();
		food_x = possible_positions[index].x;
		food_y = possible_positions[index].y;
		food_dest = { food_x, food_y, GRID, GRID };
		food = true;
	}

	// ������ ���� ����
	random_index = rand() % food_texturesv.size();
	food_texture = food_texturesv[random_index];

	// ������ ��ġ�� ũ�� ����
	food_rect = { 0, 0, spriteWidth, spriteHeight };
}


void Snake::Update()
{
	if (!start) return;
	if (gameover) return;

	if (current_direction == 3) { // ���������� �̵�
		rotation_angle = 90.0;
	}
	else if (current_direction == 2) { // �������� �̵�
		rotation_angle = -90.0;
	}
	else if (current_direction == 1) { // �Ʒ��� �̵�
		rotation_angle = 180.0;
	}
	else if (current_direction == 0) { // ���� �̵�
		rotation_angle = 0.0;
	}

	// ���� ���� �κ��� ȸ�� ������ �����մϴ�.
	auto tail_iter = std::prev(snake.end());
	auto prev_tail_iter = std::prev(tail_iter);
	if ((*tail_iter).x < (*prev_tail_iter).x) { // �������� �̵�
		tail_rotation_angle = 90.0;
	}
	else if ((*tail_iter).x > (*prev_tail_iter).x) { // ���������� �̵�
		tail_rotation_angle = -90.0;
	}
	else if ((*tail_iter).y < (*prev_tail_iter).y) { // ���� �̵�
		tail_rotation_angle = 180.0;
	}
	else if ((*tail_iter).y > (*prev_tail_iter).y) { // �Ʒ��� �̵�
		tail_rotation_angle = 0.0;
	}

	static int updateCounter = 0;  // Counter to control snake movement speed
	// Delay between each movement update
	if (updateCounter >= movementDelay) {
		updateCounter = 0;


		//������
		if (!key_queue.empty()) {
			int key = key_queue.front(); // ť���� ���� �������� �Էµ� Ű�� �����ɴϴ�.
			key_queue = std::queue<int>(); // ť���� Ű�� �����մϴ�.

			if (key == 0) { // ���� �̵�
				snake_y = -GRID;
				snake_x = 0;
				current_direction = 0;
			}
			else if (key == 1) { // �Ʒ��� �̵�
				snake_y = GRID;
				snake_x = 0;
				current_direction = 1;
			}
			else if (key == 2) { // �������� �̵�
				snake_x = -GRID;
				snake_y = 0;
				current_direction = 2;
			}
			else if (key == 3) { // ���������� �̵�
				snake_x = GRID;
				snake_y = 0;
				current_direction = 3;
			}
		}

		int new_x = snake.front().x + (snake_x / GRID * GRID);
		int new_y = snake.front().y + (snake_y / GRID * GRID);

		// ���� ���� ����� Ȯ��
		for (auto iter = std::next(snake.begin()); iter != snake.end(); iter++) {
			if ((*iter).x == snake.front().x + snake_x && (*iter).y == snake.front().y + snake_y) {
				gameover = true; // ���� ����
				Mix_PauseMusic(); // ���� �Ͻ� ����
				Mix_PlayChannelTimed(-1, die_sound, 0, 1000); // �״� �Ҹ� ���
				return; // �Լ� ����
			}
		}
		// ���� ���� �ε������� Ȯ��
		if (snake.front().x + snake_x < 0 || snake.front().x + snake_x >= SCREEN_WIDTH ||
			snake.front().y + snake_y < 0 || snake.front().y + snake_y >= SCREEN_HEIGHT) {
			gameover = true; // ���� ����
			Mix_PauseMusic(); // ���� �Ͻ� ����
			Mix_PlayChannelTimed(-1, die_sound, 0, 1000); // �״� �Ҹ� ���
			return; // �Լ� ����
		}

		snake.push_front({ new_x, new_y, GRID, GRID });

		// ���ĸԱ�
		if (snake.front().x / GRID == food_x / GRID && snake.front().y / GRID == food_y / GRID) {
			if (score <= 3) {
				movementDelay = 30;
			}
			else if (score <= 5) {
				movementDelay = 20;
			}
			else if (score <= 8) {
				movementDelay = 15;
			}
			else if (score <= 10) {
				movementDelay = 12;
			}
			else if (score <= 15) {
				movementDelay = 10;
			}
			else {
				movementDelay = 8;
			}
			score++;
			food = false;
			GenerateFood();
			Mix_PlayChannel(-1, eat_sound, 0);
		}
		else {
			snake.pop_back();
		}


		// ������ �� Ű �Է� ���� �ʱ�ȭ
		key_left = false;
		key_right = false;
		key_up = false;
		key_down = false;
	}
	else {
		updateCounter++;
	}
}

Snake::~Snake()
{
	SDL_RenderClear(g_renderer);
	SDL_DestroyTexture(bg_texture);
	SDL_DestroyTexture(snake_texture);
	SDL_DestroyTexture(ready_texture);
	SDL_DestroyTexture(Gameover_texture);
	SDL_DestroyTexture(food_texture);
	SDL_DestroyTexture(snake_head_texture);
	SDL_DestroyTexture(snake_body_texture);
	SDL_DestroyTexture(snake_tail_texture);
	Mix_FreeMusic(music);
	Mix_FreeChunk(eat_sound);
	Mix_FreeChunk(die_sound);
	TTF_CloseFont(font);
	for (auto texture : food_texturesv) {
		SDL_DestroyTexture(texture);
	}
}

void Snake::Reset()
{

	Mix_PlayMusic(music, -1);
	//���� ���� ����
	Mix_VolumeMusic(10);

	start = false;
	key_left = false;
	key_right = false;
	key_up = false;
	key_down = false;
	score = 0;
	key_queue = std::queue<int>();

	movementDelay = 30;

	/* ������ ������ ��ġ�� �̸� ���
	for (int x = 0; x < 800; x += GRID) {
		for (int y = 0; y < 600; y += GRID) {
			bool is_free = true;
			for (const auto& part : snake) {
				if (part.x == x && part.y == y) {
					is_free = false;
					break;
				}
			}
			if (is_free) {
				free_points.push_back({ x, y });
			}
		}
	}*/
	current_direction = 0;
	rotation_angle = 0.0;
	tail_rotation_angle = 0.0;
	SDL_RenderCopy(g_renderer, ready_texture, &ready_rect, &ready_dest);
	snake.clear();

	ch_x = 400, ch_y = 400;
	snake.push_front({ ch_x, ch_y,GRID,GRID });
	snake.push_back({ ch_x, ch_y + GRID, GRID,GRID });

	gameover = false;

	SDL_RenderCopy(g_renderer, bg_texture, &bg_source_rect, &bg_dest_rect);

	food = false;
	GenerateFood(); // ���� �����

	SDL_RenderCopy(g_renderer, food_texture, &food_rect, &food_dest);
	SDL_RenderCopy(g_renderer, snake_texture, &snake_rect, &(snake.front()));


}



void Snake::Render()
{
	// ��� 
	SDL_RenderCopy(g_renderer, bg_texture, &bg_source_rect, &bg_dest_rect); // clear the renderer to the draw color


	//����
	if (food) {
		SDL_RenderCopy(g_renderer, food_texture, &food_rects[random_index], &food_dest); // ����� �κ�
	}



	// ���� �׸� ��
	for (auto iter = snake.begin(); iter != snake.end(); iter++) {
		if (iter == snake.begin()) { // ���� �Ӹ��� �׸��ϴ�.
			SDL_RenderCopyEx(g_renderer, snake_head_texture, &snake_rect, &(*iter), rotation_angle, NULL, SDL_FLIP_NONE);
		}
		else if (iter == std::prev(snake.end())) { // ���� ������ �׸��ϴ�.
			SDL_RenderCopyEx(g_renderer, snake_tail_texture, &snake_rect, &(*iter), tail_rotation_angle, NULL, SDL_FLIP_NONE);
		}
		else { // ���� ������ �׸��ϴ�.
			SDL_RenderCopy(g_renderer, snake_body_texture, &snake_rect, &(*iter));
		}
	}
	// ������ ���ڿ��� ��ȯ�մϴ�.
	std::string score_text = "Score: " + std::to_string(score);

	// �ؽ�Ʈ ǥ���� �����մϴ�.
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, score_text.c_str(), color);

	// �ؽ�Ʈ �ؽ�ó�� �����մϴ�.
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(g_renderer, text_surface);


	// �ؽ�Ʈ�� �������� �׸��ϴ�.
	SDL_RenderCopy(g_renderer, text_texture, NULL, &text_rect);

	// �ؽ�ó�� ǥ���� �����մϴ�.
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	//����
	if (!start) {
		SDL_RenderCopy(g_renderer, ready_texture, &ready_rect, &ready_dest);
		SDL_RenderPresent(g_renderer);
		return;
	}
	//���ӿ���
	if (gameover) {
		SDL_RenderCopy(g_renderer, Gameover_texture, &Gameover_rect, &Gameover_dest);
	}

	SDL_RenderPresent(g_renderer); // draw to the screen
}

// HandleEvents �Լ� ����
std::queue<int> key_queue; // Ű �Է��� �����ϴ� ť

void Snake::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				if (!start) {
					start = true;
				}
				if (current_direction != 1 && (key_queue.empty() || key_queue.back() != 0)) { // ���� �Ʒ��� ���� ���� �ʰ�, ���������� �Էµ� Ű�� �� ������ �ƴ� ���� ���� �̵� ����
					key_queue.push(0); // ���� �̵��ϴ� Ű�� ť�� �߰�
				}
				break;
			case SDLK_DOWN:
				if (!start) {
					start = true;
				}
				if (current_direction != 0 && (key_queue.empty() || key_queue.back() != 1)) { // ���� ���� ���� ���� �ʰ�, ���������� �Էµ� Ű�� �Ʒ� ������ �ƴ� ���� �Ʒ��� �̵� ����
					key_queue.push(1); // �Ʒ��� �̵��ϴ� Ű�� ť�� �߰�
				}
				break;
			case SDLK_LEFT:
				if (!start) {
					start = true;
				}
				if (current_direction != 3 && (key_queue.empty() || key_queue.back() != 2)) { // ���� ���������� ���� ���� �ʰ�, ���������� �Էµ� Ű�� ���� ������ �ƴ� ���� �������� �̵� ����
					key_queue.push(2); // �������� �̵��ϴ� Ű�� ť�� �߰�
				}
				break;
			case SDLK_RIGHT:
				if (!start) {
					start = true;
				}
				if (current_direction != 2 && (key_queue.empty() || key_queue.back() != 3)) { // ���� �������� ���� ���� �ʰ�, ���������� �Էµ� Ű�� ������ ������ �ƴ� ���� ���������� �̵� ����
					key_queue.push(3); // ���������� �̵��ϴ� Ű�� ť�� �߰�
				}
				break;
			}
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (gameover) {
					start = true;
					Reset();
					Mix_ResumeMusic();
				}
			}
		default:
			break;
		}
	}
}