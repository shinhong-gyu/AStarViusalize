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


		// 화면 밖을 클릭했을때 화면 안으로 Clamp.
		if (target.x >= 40) target.x = 39;
		if (target.y >= 25) target.y = 24;
		if (target.x < 0) target.x = 0;
		if (target.y < 0) target.y = 0;


		if (dynamic_cast<DemoLevel*>(Engine::Get().GetMainLevel())->GetPlayer()->grid[target.y][target.x] == 1) return;

		position = target;
	}
}