#include "Engine/Engine.h"
#include "Level/DemoLevel.h"

#include <Windows.h>

int main()
{
	HWND consoleWindow = GetConsoleWindow();

	int width = 1920;
	int height = 1080;
	GetSystemMetrics();

	MoveWindow(consoleWindow, 0, 0, 1920, 1080, TRUE);
}
// 
// 	Engine engine;
// 	engine.LoadLevel(new DemoLevel());
// 	engine.Run();
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
