#pragma once

#include "Level/Level.h"
#include <vector>

class Start;
class Player;

class DemoLevel : public Level
{
	// RTTI 정의.
	RTTI_DECLARATIONS(DemoLevel, Level)

public:
	DemoLevel();
	~DemoLevel();
	virtual void Draw() override;

	Start* GetStartPoint();

	// 플레이어가 목표지점에 도착하면 경로와 타겟 삭제
	void PlayerArrived();

	// 움직이는 도중에 새로운 목표지점이 설정되었을 때
	void NewTargetSetting();

	// 플레이어의 랜더링 순서를 제일 뒤로 미룸
	//void PlayerReplace();

	//virtual void AddActor(Actor* newActor) override;

	Player* GetPlayer() const { return player; }

private:
	Start* startPoint;

	Player* player;
};