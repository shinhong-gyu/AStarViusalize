#include "Engine/Engine.h"
#include "Level/DemoLevel.h"

#include <Windows.h>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	const int width = 192;   // 열의 개수
	const int height = 63;  // 행의 개수

	ofstream file("map.txt");
	if (!file) {
		cerr << "파일 생성에 실패했습니다." << endl;
		return 1;
	}

	srand(static_cast<unsigned int>(time(nullptr)));

	// 맵 생성: y는 행, x는 열
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// 외곽은 모두 1로 설정
			if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
				file << "1";
			}
			else {
				// 내부: 15% 확률로 장애물(1), 그 외는 0(이동 가능)
				int r = rand() % 100; // 0 ~ 99 사이의 난수
				if (r < 15) { // 약 15% 확률
					file << "1";
				}
				else {
					file << "0";
				}
			}
		}
		file << "\n";
	}

	file.close();


	HWND consoleWindow = GetConsoleWindow();           // 화면에 보이는 콘솔 창(윈도우) 핸들

	int screenWidth = GetSystemMetrics(0);
	int screenHeight = screenWidth / 16 * 9;

	MoveWindow(consoleWindow, (screenWidth - 450) / 2, (screenHeight - 450) / 2, 450, 450, true);

	//SetWindowPos(consoleWindow, 0, 0, 0, width, height, SWP_NOZORDER);

	Engine engine;
	engine.LoadLevel(new DemoLevel());
	engine.Run();
}
// 
// 	// 
	// 	HWND consoleWindow = GetConsoleWindow();
	// 
	// 	if (consoleWindow == NULL) {
	// 		std::cout << "콘솔 창을 찾을 수 없습니다!" << std::endl;
	// 		return 1;
	// 	}
	// 
	// 	int x = 100, y = 100;
	// 	int width = 800, height = 600;
	// 
	// 	// 콘솔 창 이동
	// 	MoveWindow(consoleWindow, x, y, width, height, TRUE);
	// 
	// 	while (true) {
	// 		if (GetAsyncKeyState(VK_UP))
	// 			y -= 10;
	// 		else if (GetAsyncKeyState(VK_DOWN))
	// 			y += 10;
	// 		else if (GetAsyncKeyState(VK_LEFT))
	// 			x -= 10;
	// 		else if (GetAsyncKeyState(VK_RIGHT))
	// 			x += 10;
	// 
	// 		// ESC 키로 종료
	// 		if (GetAsyncKeyState(VK_ESCAPE)) break;
	// 
	// 		// 콘솔 창 이동
	// 		MoveWindow(consoleWindow, x, y, width, height, TRUE);
	// 		Sleep(50); // 너무 빠르게 움직이지 않도록 딜레이
	// 	}
