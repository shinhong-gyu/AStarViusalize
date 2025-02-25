#include "AStarPath.h"

AStarPath::AStarPath(const Vector2& position)
	:DrawableActor("+")
{
	this->position = position;
}

AStarPath::~AStarPath()
{
}

void AStarPath::Update(float delatTime)
{
	DrawableActor::Update(delatTime);
}
