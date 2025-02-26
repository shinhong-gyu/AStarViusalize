#include "Engine/Engine.h"
#include "Level/DemoLevel.h"

#include <Windows.h>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	const int width = 192;   // ���� ����
	const int height = 63;  // ���� ����

	ofstream file("map.txt");
	if (!file) {
		cerr << "���� ������ �����߽��ϴ�." << endl;
		return 1;
	}

	srand(static_cast<unsigned int>(time(nullptr)));

	// �� ����: y�� ��, x�� ��
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// �ܰ��� ��� 1�� ����
			if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
				file << "1";
			}
			else {
				// ����: 15% Ȯ���� ��ֹ�(1), �� �ܴ� 0(�̵� ����)
				int r = rand() % 100; // 0 ~ 99 ������ ����
				if (r < 15) { // �� 15% Ȯ��
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


	HWND consoleWindow = GetConsoleWindow();           // ȭ�鿡 ���̴� �ܼ� â(������) �ڵ�

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
	// 		std::cout << "�ܼ� â�� ã�� �� �����ϴ�!" << std::endl;
	// 		return 1;
	// 	}
	// 
	// 	int x = 100, y = 100;
	// 	int width = 800, height = 600;
	// 
	// 	// �ܼ� â �̵�
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
	// 		// ESC Ű�� ����
	// 		if (GetAsyncKeyState(VK_ESCAPE)) break;
	// 
	// 		// �ܼ� â �̵�
	// 		MoveWindow(consoleWindow, x, y, width, height, TRUE);
	// 		Sleep(50); // �ʹ� ������ �������� �ʵ��� ������
	// 	}
