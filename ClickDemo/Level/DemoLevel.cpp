#include "DemoLevel.h"
#include "Actor/Player.h"
#include "Actor/Start.h"
#include "Engine/Engine.h"
#include "Actor/AStarPath.h"
#include "Actor/Target.h"

#include <utility>
#include "Actor/Wall.h"


DemoLevel::DemoLevel()
{
	startPoint = new Start();
	player = new Player();

	AddActor(startPoint);

	AddActor(player);

	for (int y = 0; y < player->grid.size(); y++)
	{
		for (int x = 0; x < player->grid[0].size(); x++)
		{
			if (player->grid[y][x] == 1)
			{
				AddActor(new Wall({ x,y }));
			}
		}
	}
}

DemoLevel::~DemoLevel()
{
}

void DemoLevel::Draw()
{
	Super::Draw();

	if (player)
	{
		player->Draw();
	}
}

Start* DemoLevel::GetStartPoint()
{
	return startPoint;
}

// 도착하면 표시한 경로와 목표를 지움
void DemoLevel::PlayerArrived()
{
	for (int ix = 0; ix < (int)actors.size(); ++ix)
	{
		auto* actor = actors[ix];

		if (dynamic_cast<AStarPath*>(actor))
		{
			actors.erase(actors.begin() + ix);
			ix--;
			actor->Destroy();
		}

		else if (dynamic_cast<Target*>(actor))
		{
			actors.erase(actors.begin() + ix);
			ix--;
			actor->Destroy();
		}
	}
}

void DemoLevel::NewTargetSetting()
{
	for (int ix = 0; ix < (int)actors.size(); ++ix)
	{
		auto* actor = actors[ix];

		if (dynamic_cast<Target*>(actor))
		{
			actors.erase(actors.begin() + ix);
			actor->Destroy();
			return;
		}
	}
}


