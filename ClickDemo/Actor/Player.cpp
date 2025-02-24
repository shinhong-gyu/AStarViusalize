#include <iostream>

#include "Player.h"
#include "Engine/Engine.h"
#include "Level/DemoLevel.h"
#include "Algorithm/AStar.h"
#include "Start.h"
#include "Algorithm/Node.h"
#include <winbase.h>

Player::Player()
	: DrawableActor("e")
{
	color = Color::Green;

	position = Vector2(0, 0);

	pathFinder = new AStar();

	timer->SetTime(0.1);
}

Player::~Player()
{
	delete pathFinder;
	delete timer;
}

void Player::Update(float deltaTime)
{
	DrawableActor::Update(deltaTime);

	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		Engine::Get().QuitGame();
	}

	// ��Ŭ���Ǹ� ���� �������� �÷��̾� ��ġ ����
	if (Engine::Get().GetKeyDown(VK_LBUTTON))
	{

		auto* mainLevel = Engine::Get().GetMainLevel();

		if (auto* demoLevel = dynamic_cast<DemoLevel*>(mainLevel))
		{
			position = demoLevel->GetStartPoint()->GetPoisiton();
		}

		// ��Ŭ���� �Ǹ� ��ġ�� �����ϰ� �������� ����
		bOnMove = false;
		moveCount = 0;
	}

	// ��Ŭ�� �� ���콺 ��ġ�� ���� �ִܰ�� ã��.
	if (Engine::Get().GetKeyDown(VK_RBUTTON))
	{
		bOnMove = !bOnMove;

		pathFinder = new AStar();

		path.clear();

		Node* startNode = new Node(this->position);

		Vector2 target = Engine::Get().MousePosition();

		if (target.x >= 40) target.x = 39;
		if (target.y >= 25) target.y = 24;
		if (target.x < 0) target.x = 0;
		if (target.y < 0) target.y = 0;


		Node* goalNode = new Node(target);

		COORD pos;
		pos.X = target.x;
		pos.Y = target.y;

		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleCursorPosition(handle, { 0,25 });

		std::cout << target.x << ", " << target.y << std::endl;

		SetConsoleCursorPosition(handle, { 0,0 });

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
	}

}

void Player::MoveAlongPath( )
{
	if (path.empty())
	{
		return;
	}

	if (timer->IsTimeOut())
	{
		this->position = path[moveCount++]->GetPosition();
		timer->Reset();
	}

}
