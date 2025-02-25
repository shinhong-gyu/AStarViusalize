#include <iostream>

#include "Player.h"
#include "Engine/Engine.h"
#include "Level/DemoLevel.h"
#include "Algorithm/AStar.h"
#include "Start.h"
#include "Algorithm/Node.h"
#include <winbase.h>
#include "Target.h"
#include "AStarPath.h"

DemoLevel* Player::demoLevel = nullptr;

Player::Player()
	: DrawableActor("e")
{
	color = Color::Green;

	position = Vector2(1, 1);

	pathFinder = new AStar();

	timer->SetTime(0.1f);
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

	// ��Ŭ���Ǹ� ���� �������� �÷��̾� ��ġ ����
	if (Engine::Get().GetKeyDown(VK_LBUTTON))
	{

		if (demoLevel != nullptr)
		{
			demoLevel->PlayerArrived();
			position = demoLevel->GetStartPoint()->GetPoisiton();
			prePosition = position;
		}

		// ��Ŭ���� �Ǹ� ��ġ�� �����ϰ� �������� ����
		bOnMove = false;
		moveCount = 0;
	}

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


		// ȭ�� ���� Ŭ�������� ȭ�� ������ Clamp.
		if (target.x >= 40) target.x = 39;
		if (target.y >= 25) target.y = 24;
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
