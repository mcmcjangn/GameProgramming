#include "GameFunc.h"
#include <windows.h>

int g_input;
std::string g_output;

int g_X;
int g_Y;

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
	g_input = 0;
	g_output = "*";
	g_flag_running = true;

	g_missile_output = "!";
	g_flag_missile = false;

	g_X = 0;
	g_Y = 10;
	g_elapsed_time_ms = 0;

	// std::cout ��¿� ���۸� ����Ͽ�, ��� �ӵ��� �������� ������ ���Ѵ�.
	setvbuf(stdout, NULL, _IOLBF, 4096);

	// Clear the console screen.
	// ǥ����� ȭ���� ������ �����.
	system("cls");
}






/////////////////////////////////////////////////////////////
// Update() 
// ������ ������ ������Ʈ�ϴ� �Լ�.
// ���� ������ ���� �����ؾ��ϴ� ��.
// ���ӿ��� �Ͼ�� ��ȭ�� ��� �� ������ �����Ѵ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Update() {
	//���� �̵�
	if (g_input == 1) {
		g_X-=1;
		if (g_X < 0) {
			g_X = 0;
		}
	}
	//���� �̵�
	else if (g_input == 2) {
		g_X+=1;
		if (g_X > 29) {
			g_X = 29;
		}
	}
	//���� �̵�
	else if (g_input == 4) {
		g_Y -= 1;
		if (g_Y < 0) {
			g_Y = 0;
		}
	}
	//�Ʒ��� �̵�
	else if (g_input == 5) {
		g_Y += 1;
		if (g_Y > 19) {
			g_Y = 19;
		}
	}
	//�̻��� �߻�
	else if (g_input == 3) {
		if (g_flag_missile == false) {
			g_flag_missile = true;
			g_missile_Y = g_Y;
			g_missile_X = g_X;
		}
	}

	//�̻��� �̵�
	if (g_flag_missile == true) {
		g_missile_Y -= 1;
		if (g_missile_Y < 0) {
			g_flag_missile = false;
		}
	}
	g_elapsed_time_ms += 33;
}




/////////////////////////////////////////////////////////////
// Render() 
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Render() {
	//// 1. ��� �׸���.
	// 1.1. Ŀ���� �ܼ� ȭ���� ���� �� �𼭸� �κ����� �ű��. ��ǥ(0, 0)
	// <windows.h>���� �����ϴ� �Լ��� ����Ѵ�.
	COORD cur;
	cur.X = 0;
	cur.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);

	COORD cur_missile;
	cur_missile.X = 0;
	cur_missile.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur_missile);

	//// 1.2. ��� �κ��� '_'���� ä���.
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 30; j++) {
			std::cout << "_";
		}
		std::cout << std::endl;
	}

	//// 1.3. ��� �Ʒ��� �ð��� ǥ���Ѵ�,
	std::cout << "Elapsed Time: " << g_elapsed_time_ms / 1000.0f << std::endl;


	//// 2. ĳ���� �׸���.
	// 2.1. Ŀ���� ĳ���Ͱ� �׷��� ��ġ�� �ű��. 
	cur.X = g_X;
	cur.Y = g_Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);

	// 2.2. ĳ���� ǥ���� �׸���.
	std::cout << g_output;


	std::cout.flush();

	//�̻��� �׸���
	if (g_flag_missile == true) {
		cur_missile.X = g_missile_X;
		cur_missile.Y = g_missile_Y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur_missile);
		std::cout << g_missile_output;
	}

	//// 3. ������ �ϼ�.
	// std::cout���� ����� ���� ��, ���� ȭ�鿡 ǥ�õ� �ʰ� ���ۿ� ���� �ִ� ���� �ִٸ�, ��� ȭ�鿡 ��µǵ��� �Ѵ�.
	std::cout.flush();
}



/////////////////////////////////////////////////////////////
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
				g_input = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_input = 2;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {
				g_input = 3;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				g_input = 4;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				g_input = 5;
			}
			break;

		case SDL_KEYUP:
			g_input = 0;
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
}
