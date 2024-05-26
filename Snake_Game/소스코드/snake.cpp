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
	//배경음악
	music = Mix_LoadMUS("../Resources/bgm.mp3");
	if (music == NULL) {
		std::cout << "Mix_LoadMUS: " << Mix_GetError() << std::endl;
		exit(1);
	}
	//음식 먹는 소리
	eat_sound = Mix_LoadWAV("../Resources/eat.wav");
	if (eat_sound == NULL) {
		std::cout << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
		exit(1);
	}

	//죽는 소리
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

	// 스프라이트 시트 
	SDL_Surface* food_sheet_surface = IMG_Load("../Resources/seaAnimals/seaCreature.png");
	SDL_Texture* food_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, food_sheet_surface);
	SDL_FreeSurface(food_sheet_surface);

	//food_texturesv 벡터에 추가
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

	// snake.cpp 파일에서 뱀의 머리 텍스처를 로드합니다.
	SDL_Surface* snake_head_surface = IMG_Load("../Resources/snake_head.png");
	snake_head_texture = SDL_CreateTextureFromSurface(g_renderer, snake_head_surface);
	SDL_FreeSurface(snake_head_surface);
	// snake.cpp 파일에서 뱀의 몸통 텍스처를 로드합니다.
	SDL_Surface* snake_body_surface = IMG_Load("../Resources/snake_body.png");
	snake_body_texture = SDL_CreateTextureFromSurface(g_renderer, snake_body_surface);
	SDL_FreeSurface(snake_body_surface);
	// snake.cpp 파일에서 뱀의 꼬리 텍스처를 로드합니다.
	SDL_Surface* snake_tail_surface = IMG_Load("../Resources/snake_tail.png");
	snake_tail_texture = SDL_CreateTextureFromSurface(g_renderer, snake_tail_surface);
	SDL_FreeSurface(snake_tail_surface);

	//Ready
	SDL_Surface* ready_surface = IMG_Load("../Resources/Ready.png");
	SDL_SetSurfaceAlphaMod(ready_surface, 200); // 투명도 값을 설정 (0~255 범위, 0이 완전 투명, 255가 완전 불투명)
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

	//score 폰트
	font = TTF_OpenFont("../Resources/pxfont.ttf", 24);
	color = { 0, 98, 98 };
	// 텍스트의 위치와 크기 설정
	text_rect.x = 10;
	text_rect.y = 10;
	text_rect.w = 100;
	text_rect.h = 50;

	food = false;
	GenerateFood();
}

void Snake::GenerateFood()
{
	// 가능한 모든 위치를 찾습니다.
	std::vector<SDL_Rect> possible_positions;
	for (int x = 0; x < SCREEN_WIDTH; x += GRID) {
		for (int y = 0; y < SCREEN_HEIGHT; y += GRID) {
			possible_positions.push_back({ x, y, GRID, GRID });
		}
	}

	// 뱀의 위치를 제외합니다.
	for (const auto& part : snake) {
		possible_positions.erase(std::remove_if(possible_positions.begin(), possible_positions.end(),
			[&part](const SDL_Rect& rect) {
				return rect.x == part.x && rect.y == part.y;
			}), possible_positions.end());
	}

	// 가능한 위치 중에서 랜덤하게 선택합니다.
	if (!possible_positions.empty()) {
		int index = rand() % possible_positions.size();
		food_x = possible_positions[index].x;
		food_y = possible_positions[index].y;
		food_dest = { food_x, food_y, GRID, GRID };
		food = true;
	}

	// 랜덤한 음식 선택
	random_index = rand() % food_texturesv.size();
	food_texture = food_texturesv[random_index];

	// 음식의 위치와 크기 설정
	food_rect = { 0, 0, spriteWidth, spriteHeight };
}


void Snake::Update()
{
	if (!start) return;
	if (gameover) return;

	if (current_direction == 3) { // 오른쪽으로 이동
		rotation_angle = 90.0;
	}
	else if (current_direction == 2) { // 왼쪽으로 이동
		rotation_angle = -90.0;
	}
	else if (current_direction == 1) { // 아래로 이동
		rotation_angle = 180.0;
	}
	else if (current_direction == 0) { // 위로 이동
		rotation_angle = 0.0;
	}

	// 뱀의 꼬리 부분의 회전 각도를 결정합니다.
	auto tail_iter = std::prev(snake.end());
	auto prev_tail_iter = std::prev(tail_iter);
	if ((*tail_iter).x < (*prev_tail_iter).x) { // 왼쪽으로 이동
		tail_rotation_angle = 90.0;
	}
	else if ((*tail_iter).x > (*prev_tail_iter).x) { // 오른쪽으로 이동
		tail_rotation_angle = -90.0;
	}
	else if ((*tail_iter).y < (*prev_tail_iter).y) { // 위로 이동
		tail_rotation_angle = 180.0;
	}
	else if ((*tail_iter).y > (*prev_tail_iter).y) { // 아래로 이동
		tail_rotation_angle = 0.0;
	}

	static int updateCounter = 0;  // Counter to control snake movement speed
	// Delay between each movement update
	if (updateCounter >= movementDelay) {
		updateCounter = 0;


		//움직임
		if (!key_queue.empty()) {
			int key = key_queue.front(); // 큐에서 가장 마지막에 입력된 키를 가져옵니다.
			key_queue = std::queue<int>(); // 큐에서 키를 제거합니다.

			if (key == 0) { // 위로 이동
				snake_y = -GRID;
				snake_x = 0;
				current_direction = 0;
			}
			else if (key == 1) { // 아래로 이동
				snake_y = GRID;
				snake_x = 0;
				current_direction = 1;
			}
			else if (key == 2) { // 왼쪽으로 이동
				snake_x = -GRID;
				snake_y = 0;
				current_direction = 2;
			}
			else if (key == 3) { // 오른쪽으로 이동
				snake_x = GRID;
				snake_y = 0;
				current_direction = 3;
			}
		}

		int new_x = snake.front().x + (snake_x / GRID * GRID);
		int new_y = snake.front().y + (snake_y / GRID * GRID);

		// 뱀의 몸에 닿는지 확인
		for (auto iter = std::next(snake.begin()); iter != snake.end(); iter++) {
			if ((*iter).x == snake.front().x + snake_x && (*iter).y == snake.front().y + snake_y) {
				gameover = true; // 게임 종료
				Mix_PauseMusic(); // 음악 일시 중지
				Mix_PlayChannelTimed(-1, die_sound, 0, 1000); // 죽는 소리 재생
				return; // 함수 종료
			}
		}
		// 뱀이 벽에 부딪히는지 확인
		if (snake.front().x + snake_x < 0 || snake.front().x + snake_x >= SCREEN_WIDTH ||
			snake.front().y + snake_y < 0 || snake.front().y + snake_y >= SCREEN_HEIGHT) {
			gameover = true; // 게임 종료
			Mix_PauseMusic(); // 음악 일시 중지
			Mix_PlayChannelTimed(-1, die_sound, 0, 1000); // 죽는 소리 재생
			return; // 함수 종료
		}

		snake.push_front({ new_x, new_y, GRID, GRID });

		// 음식먹기
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


		// 움직임 후 키 입력 상태 초기화
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
	//음악 볼륨 조절
	Mix_VolumeMusic(10);

	start = false;
	key_left = false;
	key_right = false;
	key_up = false;
	key_down = false;
	score = 0;
	key_queue = std::queue<int>();

	movementDelay = 30;

	/* 가능한 음식의 위치를 미리 계산
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
	GenerateFood(); // 음식 재생성

	SDL_RenderCopy(g_renderer, food_texture, &food_rect, &food_dest);
	SDL_RenderCopy(g_renderer, snake_texture, &snake_rect, &(snake.front()));


}



void Snake::Render()
{
	// 배경 
	SDL_RenderCopy(g_renderer, bg_texture, &bg_source_rect, &bg_dest_rect); // clear the renderer to the draw color


	//음식
	if (food) {
		SDL_RenderCopy(g_renderer, food_texture, &food_rects[random_index], &food_dest); // 변경된 부분
	}



	// 뱀을 그릴 때
	for (auto iter = snake.begin(); iter != snake.end(); iter++) {
		if (iter == snake.begin()) { // 뱀의 머리를 그립니다.
			SDL_RenderCopyEx(g_renderer, snake_head_texture, &snake_rect, &(*iter), rotation_angle, NULL, SDL_FLIP_NONE);
		}
		else if (iter == std::prev(snake.end())) { // 뱀의 꼬리를 그립니다.
			SDL_RenderCopyEx(g_renderer, snake_tail_texture, &snake_rect, &(*iter), tail_rotation_angle, NULL, SDL_FLIP_NONE);
		}
		else { // 뱀의 몸통을 그립니다.
			SDL_RenderCopy(g_renderer, snake_body_texture, &snake_rect, &(*iter));
		}
	}
	// 점수를 문자열로 변환합니다.
	std::string score_text = "Score: " + std::to_string(score);

	// 텍스트 표면을 생성합니다.
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, score_text.c_str(), color);

	// 텍스트 텍스처를 생성합니다.
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(g_renderer, text_surface);


	// 텍스트를 렌더러에 그립니다.
	SDL_RenderCopy(g_renderer, text_texture, NULL, &text_rect);

	// 텍스처와 표면을 해제합니다.
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	//레디
	if (!start) {
		SDL_RenderCopy(g_renderer, ready_texture, &ready_rect, &ready_dest);
		SDL_RenderPresent(g_renderer);
		return;
	}
	//게임오버
	if (gameover) {
		SDL_RenderCopy(g_renderer, Gameover_texture, &Gameover_rect, &Gameover_dest);
	}

	SDL_RenderPresent(g_renderer); // draw to the screen
}

// HandleEvents 함수 수정
std::queue<int> key_queue; // 키 입력을 저장하는 큐

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
				if (current_direction != 1 && (key_queue.empty() || key_queue.back() != 0)) { // 뱀이 아래로 가고 있지 않고, 마지막으로 입력된 키가 위 방향이 아닐 때만 위로 이동 가능
					key_queue.push(0); // 위로 이동하는 키를 큐에 추가
				}
				break;
			case SDLK_DOWN:
				if (!start) {
					start = true;
				}
				if (current_direction != 0 && (key_queue.empty() || key_queue.back() != 1)) { // 뱀이 위로 가고 있지 않고, 마지막으로 입력된 키가 아래 방향이 아닐 때만 아래로 이동 가능
					key_queue.push(1); // 아래로 이동하는 키를 큐에 추가
				}
				break;
			case SDLK_LEFT:
				if (!start) {
					start = true;
				}
				if (current_direction != 3 && (key_queue.empty() || key_queue.back() != 2)) { // 뱀이 오른쪽으로 가고 있지 않고, 마지막으로 입력된 키가 왼쪽 방향이 아닐 때만 왼쪽으로 이동 가능
					key_queue.push(2); // 왼쪽으로 이동하는 키를 큐에 추가
				}
				break;
			case SDLK_RIGHT:
				if (!start) {
					start = true;
				}
				if (current_direction != 2 && (key_queue.empty() || key_queue.back() != 3)) { // 뱀이 왼쪽으로 가고 있지 않고, 마지막으로 입력된 키가 오른쪽 방향이 아닐 때만 오른쪽으로 이동 가능
					key_queue.push(3); // 오른쪽으로 이동하는 키를 큐에 추가
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