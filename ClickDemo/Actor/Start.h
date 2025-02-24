#pragma once

#include "Actor/DrawableActor.h"

class Start : public DrawableActor
{
public:
	Start();

	virtual void Update(float deltaTime) override;

	inline Vector2 GetPoisiton() const { return position; }
};