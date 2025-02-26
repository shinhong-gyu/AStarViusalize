#include "Target.h"

Target::Target(const Vector2& position, const char* image)
	: DrawableActor(image)
{
	color = Color::Red;

	this->position = position;
}

Target::~Target()
{
}

void Target::Update(float deltaTime)
{
	DrawableActor::Update(deltaTime);
}
