#pragma once

#include "Actor/DrawableActor.h"

class Player;

class Start : public DrawableActor
{
public:
	Start();

	virtual void Update(float deltaTime) override;

	inline Vector2 GetPoisiton() const { return position; }
private:
	static Player* player;
};