
#include "Player.h"
#include "Engine/Engine.h"
#include "Level/DemoLevel.h"
#include "Algorithm/AStar.h"
#include "Start.h"
#include "Algorithm/Node.h"
#include "Target.h"
#include "AStarPath.h"

#include <winbase.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

DemoLevel* Player::demoLevel = nullptr;

Player::Player()
	: DrawableActor("e")
{
	color = Color::Green;

	pathFinder = new AStar();

	timer->SetTime(0.1f);

	ifstream inFile("map.txt");

	if (inFile)
	{
		string line;
		int row = 0;
		while (getline(inFile, line) && row < 63)
		{
			std::vector<int> temp;
			for (int col = 0; col < 192 && col < (int)line.size(); ++col)
			{
				temp.push_back(line[col] - '0');
			}
			grid.push_back(temp);
			row++;
		}
	}

	// ���÷����� ���� ũ�⸦ �޾ƿ�
	screenWidth = GetSystemMetrics(0);

	// ȭ�� �� �°� ���� ũ�� ����
	screenHeight = screenWidth / 16 * 9;

	// �÷��̾��� ������ġ�� ����� ����
	position = Vector2(((int)grid[0].size() / 2) - 1, ((int)grid.size() / 2) - 1);

	prePosition = position;

	// �ܼ� ȭ���� ����� �߾����� ����
	curWindowPos = { (GetSystemMetrics(0) - 450) / 2,((GetSystemMetrics(0) / 16 * 9 - 450)) / 2 };
}

Player::~Player()
{
	SafeDelete(pathFinder);
	SafeDelete(timer);
}

void Player::Update(float deltaTime)
{
	DrawableActor::Update(deltaTime);

	if (auto* mainLevel = Engine::Get().GetMainLevel())
	{
		if (demoLevel == nullptr)
		{
			demoLevel = dynamic_cast<DemoLevel*>(mainLevel);
		}
	}

	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		Engine::Get().QuitGame();
	}

	if (Engine::Get().GetKeyDown(VK_TAB))
	{
		bToggle = !bToggle;

		HWND consoleWindow = GetConsoleWindow();

		// �÷��̾� ��ġ 0,0���� ����
		position = { 0,0 };

		// ���, Ÿ�� ��ü�� ��� ����
		demoLevel->PlayerArrived();

		// �̹��� ��ü�� ���̵��� �ܼ� ȭ�� ũ�� ���� 
		MoveWindow(consoleWindow, 0, 0, 1500, 900, true);
	}

	if (bToggle)
	{
		HWND consoleWindow = GetConsoleWindow();

		// �̹��� ��ü�� ���̵��� �ܼ� ȭ�� ũ�� ���� 
		MoveWindow(consoleWindow, 0, 0, 1500, 850, true);

		quitTimer->Update(deltaTime);

		// Ÿ�̸Ӱ� Time Out �Ǹ� ���ӵ� ����
		if (quitTimer->IsTimeOut())
		{
			Engine::Get().QuitGame();
		}

		return;
	}

	// ��Ŭ���Ǹ� ���� �������� �÷��̾� ��ġ ����
	if (Engine::Get().GetKeyDown(VK_LBUTTON))
	{

		if (demoLevel != nullptr)
		{
			demoLevel->PlayerArrived();
			prePosition = position;
			position = demoLevel->GetStartPoint()->GetPoisiton();

			int dx = position.x - prePosition.x;
			int dy = position.y - prePosition.y;

			curWindowPos.x = curWindowPos.x + dx * 10;
			curWindowPos.y = curWindowPos.y + dy / 9 * 16 * 10;

			// �ܼ� �����찡 ȭ�� ������ ����� �ʵ��� ȭ�� �� ��ǥ�� clamp
			if (curWindowPos.x < 0) curWindowPos.x = 0;
			if (curWindowPos.y < 0) curWindowPos.y = 0;
			if (curWindowPos.y > 1080 - 450) curWindowPos.y = 1080 - 450;
			if (curWindowPos.x > 1920 - 450) curWindowPos.x = 1920 - 450;

			HWND consoleWindow = GetConsoleWindow();

			MoveWindow(consoleWindow, curWindowPos.x, curWindowPos.y, 450, 450, true);
		}

		// ��Ŭ���� �Ǹ� ��ġ�� �����ϰ� �������� ����
		bOnMove = false;
		moveCount = 0;

	}

	Vector2 offset = Engine::Get().CurrentOffset();

	// ��Ŭ�� �� ���콺 ��ġ�� ���� �ִܰ�� ã��.
	if (Engine::Get().GetKeyDown(VK_RBUTTON))
	{
		bOnMove = !bOnMove;

		if (pathFinder != nullptr)
		{
			SafeDelete(pathFinder);
		}

		pathFinder = new AStar();

		path.clear();


		Node* startNode = new Node(this->position);

		Vector2 target = Engine::Get().MousePosition();

		target.x = target.x + offset.x;
		target.y = target.y + offset.y;

		// ȭ�� ���� Ŭ�������� ȭ�� ������ Clamp.
		if (target.x >= grid[0].size()) target.x = (int)grid[0].size() - 1;
		if (target.y >= grid.size()) target.y = (int)grid.size() - 1;
		if (target.x < 0) target.x = 0;
		if (target.y < 0) target.y = 0;

		if (grid[target.y][target.x] == 1) return;

		if (demoLevel != nullptr)
		{
			// Ŭ���� ��ġ�� target ���� �߰�
			demoLevel->AddActor(new Target(target, "x"));

			// ���� ������ ��ġ�� �÷��̾��� ù ��ġ�� ����
			demoLevel->GetStartPoint()->SetPosition(this->position);

			// ���ο� ��ǥ ������ ���� ����.
			demoLevel->NewTargetSetting();
		}

		Node* goalNode = new Node(target);

		path = pathFinder->FindPath(startNode, goalNode, grid);

		bOnMove = true;
		moveCount = 0;
	}

	if (!bOnMove)
	{
		return;
	}


	if (moveCount < (int)path.size())
	{
		timer->Update(deltaTime);

		MoveAlongPath();
	}
	else
	{
		bOnMove = false;
		if (demoLevel)
		{
			demoLevel->PlayerArrived();
		}
	}
}

void Player::MoveAlongPath()
{
	// �� vector��� �ƹ��͵� ���� ����.
	if (path.empty())
	{
		// Ÿ�̸� �ʱ�ȭ
		timer->Reset();

		// �������� �ʵ��� ��
		bOnMove = false;

		// ��ο� Ÿ�� ����
		if (demoLevel)
		{
			demoLevel->PlayerArrived();
		}

		return;
	}

	if (timer->IsTimeOut())
	{
		// ��θ� ���� ������
		this->position = path[moveCount++]->GetPosition();

		if (demoLevel)
		{
			demoLevel->AddActor(new AStarPath(this->position));
		}

		// �÷��̾��� ����
		int dx = this->position.x - prePosition.x;
		int dy = this->position.y - prePosition.y;

		HWND consoleWindow = GetConsoleWindow();

		// �÷��̾��� ������ ���� �ܼ� ������ ��ġ�� ������
		curWindowPos.x = curWindowPos.x + dx * 10;
		curWindowPos.y = curWindowPos.y + dy * 10;

		// ȭ�� ���� Ŭ�������� ȭ�� ������ Clamp.
		if (curWindowPos.x < 0) curWindowPos.x = 0;
		if (curWindowPos.y < 0) curWindowPos.y = 0;
		if (curWindowPos.y > 1080 - 450) curWindowPos.y = 1080 - 450;
		if (curWindowPos.x > 1920 - 450) curWindowPos.x = 1920 - 450;

		MoveWindow(consoleWindow, curWindowPos.x, curWindowPos.y, 450, 450, true);


		prePosition = this->position;

		timer->Reset();
	}
}
