#pragma once
#pragma once
#include "Gamefunc.h"
#include <queue>

class Snake
{
public:
	Snake();
	~Snake();

	void HandleEvents();
	void Update();
	void Render();
	void Reset();
	void setSpeed(int newspeed) {
		this->speed = newspeed;
	}
	void GenerateFood();

	bool key_left;
	bool key_right;
	bool key_up;
	bool key_down;
	bool start;
	bool gameover;
	int step_size = 1;
	// snake
	list <SDL_Rect> snake;
	SDL_Texture* snake_texture; // 뱀 복사 받을 곳
	SDL_Rect snake_dest; // 뱀 현재 위치
	SDL_Rect snake_rect; // 뱀 그림에서 잘라올 사각형
	SDL_Texture* snake_head_texture; // 뱀 머리 복사 받을 곳
	SDL_Texture* snake_body_texture; // 뱀 몸통 복사 받을 곳
	SDL_Texture* snake_tail_texture; // 뱀 꼬리 복사 받을 곳
	double rotation_angle;
	double tail_rotation_angle;

	// 먹이
	std::vector<SDL_Rect> food_rects;
	vector<SDL_Texture*> food_texturesv;
	std::vector<SDL_Point> free_points;
	SDL_Texture* food_texture;
	SDL_Rect food_dest;
	SDL_Rect food_rect;

	TTF_Font* font;
	SDL_Color color;

	// 텍스트의 위치와 크기를 설정합니다.
	SDL_Rect text_rect;

	int movementDelay; // 뱀의 움직임 딜레이
	int spriteWidth = 140; // 스프라이트의 너비
	int spriteHeight = 138; // 스프라이트의 높이
	int numSpritesX = 3; // 스프라이트 시트의 가로 줄에 있는 스프라이트의 수
	int numSpritesY = 2; // 스프라이트 시트의 세로 줄에 있는 스프라이트의 수
	bool food;
	int random_index;

	const int GRID = 40;
	int score;
	int current_direction = 0;

	std::queue<int> key_queue;


	Mix_Music* music;
	Mix_Chunk* eat_sound;
	Mix_Chunk* die_sound;
private:

	const int WIDTH = 20;
	const int HEIGHT = 20;
	int speed;

	//배경
	SDL_Rect bg_dest_rect; 
	SDL_Rect bg_source_rect;
	SDL_Texture* bg_texture; 

	//Ready
	SDL_Texture* ready_texture;
	SDL_Rect ready_rect; 
	SDL_Rect ready_dest; 

	//Gameover
	SDL_Texture* Gameover_texture;
	SDL_Rect Gameover_rect; 
	SDL_Rect Gameover_dest; 

	int ch_x;
	int ch_y;

	// 위치
	int food_x, food_y;
	int snake_x, snake_y;
};
