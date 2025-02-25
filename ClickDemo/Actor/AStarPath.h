#pragma once
#include "Actor/DrawableActor.h"

class AStarPath : public DrawableActor
{
public:
	AStarPath(const Vector2& position);
	~AStarPath();

	virtual void Update(float delatTime) override;
private:
};