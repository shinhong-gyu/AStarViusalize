#include "Player.h"
#include "Engine/Engine.h"
#include "Level/DemoLevel.h"
#include "Algorithm/AStar.h"
#include "Start.h"
#include "Algorithm/Node.h"

Player::Player()
	: DrawableActor("e")
{
	color = Color::Green;

	position = Vector2(0, 0);

	pathFinder = new AStar();
}

Player::~Player()
{
	delete pathFinder;
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
		pathFinder = new AStar();

		path.clear();

		Node* startNode = new Node(this->position);

		Vector2 target = Engine::Get().MousePosition();

		if (target.x >= 40) target.x = 39;
		if (target.y >= 25) target.y = 24;
		if (target.x < 0) target.x = 0;
		if (target.y < 0) target.y = 0;


		Node* goalNode = new Node(target);

		path = pathFinder->FindPath(startNode, goalNode, grid);

		bOnMove = true;
	}

	if (!bOnMove)
	{
		return;
	}

	if (moveCount < (int)path.size())
	{
		MoveAlongPath();
	}
	else
	{
		bOnMove = false;
	}

}

void Player::MoveAlongPath()
{
	if (path.empty())
	{
		return;
	}

	this->position = path[moveCount++]->GetPosition();
	Sleep(100);

}
