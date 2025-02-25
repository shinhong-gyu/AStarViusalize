#pragma once
#include "Actor/DrawableActor.h"

class Target : public DrawableActor
{
	RTTI_DECLARATIONS(Target, DrawableActor)

public:
	Target(const Vector2& position, const char* image);
	~Target();

	virtual void Update(float deltaTime) override;

private:
};