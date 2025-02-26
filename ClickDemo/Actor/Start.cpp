#include "Start.h"
#include "Engine/Engine.h"

#include <iostream>
#include "Level/DemoLevel.h"
#include "Player.h"

Start::Start()
	: DrawableActor("s")
{
	color = Color::Red;
}

void Start::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (Engine::Get().GetKeyDown(VK_LBUTTON))
	{
		Vector2 target = Engine::Get().MousePosition();

		auto* player = dynamic_cast<DemoLevel*>(Engine::Get().GetMainLevel())->GetPlayer();

		if (player == nullptr) return;
		// 화면 밖을 클릭했을때 화면 안으로 Clamp.
		if (target.x >= player->grid[0].size()) target.x = player->grid[0].size() - 1;
		if (target.y >= player->grid.size()) target.y = player->grid.size() - 1;
		if (target.x < 0) target.x = 0;
		if (target.y < 0) target.y = 0;

		if (player->grid[target.y][target.x] == 1) return;

		position = target;
	}
}