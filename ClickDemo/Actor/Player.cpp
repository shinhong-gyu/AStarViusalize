
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

	// 디스플레이의 가로 크기를 받아옴
	screenWidth = GetSystemMetrics(0);

	// 화면 비에 맞게 새로 크기 조정
	screenHeight = screenWidth / 16 * 9;

	// 플레이어의 현재위치를 가운데로 조정
	position = Vector2(((int)grid[0].size() / 2) - 1, ((int)grid.size() / 2) - 1);

	prePosition = position;

	// 콘솔 화면을 모니터 중앙으로 조정
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

		// 플레이어 위치 0,0으로 고정
		position = { 0,0 };

		// 경로, 타겟 객체들 모두 지움
		demoLevel->PlayerArrived();

		// 이미지 전체가 보이도록 콘솔 화면 크기 고정 
		MoveWindow(consoleWindow, 0, 0, 1500, 900, true);
	}

	if (bToggle)
	{
		HWND consoleWindow = GetConsoleWindow();

		// 이미지 전체가 보이도록 콘솔 화면 크기 고정 
		MoveWindow(consoleWindow, 0, 0, 1500, 850, true);

		quitTimer->Update(deltaTime);

		// 타이머가 Time Out 되면 게임도 종료
		if (quitTimer->IsTimeOut())
		{
			Engine::Get().QuitGame();
		}

		return;
	}

	// 좌클릭되면 시작 지점으로 플레이어 위치 변경
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

			// 콘솔 윈도우가 화면 밖으로 벗어나지 않도록 화면 안 좌표로 clamp
			if (curWindowPos.x < 0) curWindowPos.x = 0;
			if (curWindowPos.y < 0) curWindowPos.y = 0;
			if (curWindowPos.y > 1080 - 450) curWindowPos.y = 1080 - 450;
			if (curWindowPos.x > 1920 - 450) curWindowPos.x = 1920 - 450;

			HWND consoleWindow = GetConsoleWindow();

			MoveWindow(consoleWindow, curWindowPos.x, curWindowPos.y, 450, 450, true);
		}

		// 좌클릭이 되면 위치만 갱신하고 움직이지 않음
		bOnMove = false;
		moveCount = 0;

	}

	Vector2 offset = Engine::Get().CurrentOffset();

	// 우클릭 시 마우스 위치로 가는 최단경로 찾기.
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

		// 화면 밖을 클릭했을때 화면 안으로 Clamp.
		if (target.x >= grid[0].size()) target.x = (int)grid[0].size() - 1;
		if (target.y >= grid.size()) target.y = (int)grid.size() - 1;
		if (target.x < 0) target.x = 0;
		if (target.y < 0) target.y = 0;

		if (grid[target.y][target.x] == 1) return;

		if (demoLevel != nullptr)
		{
			// 클릭된 위치에 target 액터 추가
			demoLevel->AddActor(new Target(target, "x"));

			// 시작 지점의 위치를 플레이어의 첫 위치로 설정
			demoLevel->GetStartPoint()->SetPosition(this->position);

			// 새로운 목표 지점이 설정 됐음.
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
	// 빈 vector라면 아무것도 하지 않음.
	if (path.empty())
	{
		// 타이머 초기화
		timer->Reset();

		// 움직이지 않도록 함
		bOnMove = false;

		// 경로와 타겟 지움
		if (demoLevel)
		{
			demoLevel->PlayerArrived();
		}

		return;
	}

	if (timer->IsTimeOut())
	{
		// 경로를 따라 움직임
		this->position = path[moveCount++]->GetPosition();

		if (demoLevel)
		{
			demoLevel->AddActor(new AStarPath(this->position));
		}

		// 플레이어의 변위
		int dx = this->position.x - prePosition.x;
		int dy = this->position.y - prePosition.y;

		HWND consoleWindow = GetConsoleWindow();

		// 플레이어의 변위를 현재 콘솔 윈도우 위치에 더해줌
		curWindowPos.x = curWindowPos.x + dx * 10;
		curWindowPos.y = curWindowPos.y + dy * 10;

		// 화면 밖을 클릭했을때 화면 안으로 Clamp.
		if (curWindowPos.x < 0) curWindowPos.x = 0;
		if (curWindowPos.y < 0) curWindowPos.y = 0;
		if (curWindowPos.y > 1080 - 450) curWindowPos.y = 1080 - 450;
		if (curWindowPos.x > 1920 - 450) curWindowPos.x = 1920 - 450;

		MoveWindow(consoleWindow, curWindowPos.x, curWindowPos.y, 450, 450, true);


		prePosition = this->position;

		timer->Reset();
	}
}
