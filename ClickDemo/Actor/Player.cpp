
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

	position = Vector2((int)grid[0].size() / 2 - 1, (int)grid.size() / 2 - 1);
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

	// 좌클릭되면 시작 지점으로 플레이어 위치 변경
	if (Engine::Get().GetKeyDown(VK_LBUTTON))
	{

		if (demoLevel != nullptr)
		{
			demoLevel->PlayerArrived();
			position = demoLevel->GetStartPoint()->GetPoisiton();
			prePosition = position;
		}

		// 좌클릭이 되면 위치만 갱신하고 움직이지 않음
		bOnMove = false;
		moveCount = 0;
	}

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
	if (path.empty())
	{
		return;
	}

	if (timer->IsTimeOut())
	{
		this->position = path[moveCount++]->GetPosition();

		if (demoLevel)
		{
			demoLevel->AddActor(new AStarPath(this->position));
		}
		//prePosition = this->position;

		timer->Reset();
	}
}
