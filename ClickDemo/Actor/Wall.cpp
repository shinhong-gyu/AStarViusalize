#include "Wall.h"

Wall::Wall(const Vector2& position)
	: DrawableActor("#")
{
	this->position = position;
}

Wall::~Wall()
{
}

void Wall::Update(float delatTime)
{
	DrawableActor::Update(delatTime);
}
