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
	SDL_Texture* snake_texture; // �� ���� ���� ��
	SDL_Rect snake_dest; // �� ���� ��ġ
	SDL_Rect snake_rect; // �� �׸����� �߶�� �簢��
	SDL_Texture* snake_head_texture; // �� �Ӹ� ���� ���� ��
	SDL_Texture* snake_body_texture; // �� ���� ���� ���� ��
	SDL_Texture* snake_tail_texture; // �� ���� ���� ���� ��
	double rotation_angle;
	double tail_rotation_angle;

	// ����
	std::vector<SDL_Rect> food_rects;
	vector<SDL_Texture*> food_texturesv;
	std::vector<SDL_Point> free_points;
	SDL_Texture* food_texture;
	SDL_Rect food_dest;
	SDL_Rect food_rect;

	TTF_Font* font;
	SDL_Color color;

	// �ؽ�Ʈ�� ��ġ�� ũ�⸦ �����մϴ�.
	SDL_Rect text_rect;

	int movementDelay; // ���� ������ ������
	int spriteWidth = 140; // ��������Ʈ�� �ʺ�
	int spriteHeight = 138; // ��������Ʈ�� ����
	int numSpritesX = 3; // ��������Ʈ ��Ʈ�� ���� �ٿ� �ִ� ��������Ʈ�� ��
	int numSpritesY = 2; // ��������Ʈ ��Ʈ�� ���� �ٿ� �ִ� ��������Ʈ�� ��
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

	//���
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

	// ��ġ
	int food_x, food_y;
	int snake_x, snake_y;
};
