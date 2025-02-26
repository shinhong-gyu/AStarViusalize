#pragma once

#include "Actor/DrawableActor.h"
#include "Engine/Timer.h"

#include <vector>

class AStar;
class Node;
class DemoLevel;

class Player : public DrawableActor
{
	RTTI_DECLARATIONS(Player, DrawableActor)

public:
	Player();
	~Player();

	virtual void Update(float deltaTime) override;

	std::vector<std::vector<int> > grid;

	void MoveAlongPath();
private:
	AStar* pathFinder;
	std::vector<Node*> path;

	bool bOnMove = false;

	int moveCount = 0;

    Timer* timer = new Timer(0.1f);

    static DemoLevel* demoLevel;

    Vector2 prePosition;
};