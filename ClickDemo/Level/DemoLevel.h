#pragma once

#include "Level/Level.h"
#include <vector>

class Start;
class Player;

class DemoLevel : public Level
{
	// RTTI ����.
	RTTI_DECLARATIONS(DemoLevel, Level)

public:
	DemoLevel();
	~DemoLevel();
	virtual void Draw() override;

	Start* GetStartPoint();

	// �÷��̾ ��ǥ������ �����ϸ� ��ο� Ÿ�� ����
	void PlayerArrived();

	// �����̴� ���߿� ���ο� ��ǥ������ �����Ǿ��� ��
	void NewTargetSetting();

	// �÷��̾��� ������ ������ ���� �ڷ� �̷�
	//void PlayerReplace();

	//virtual void AddActor(Actor* newActor) override;

	Player* GetPlayer() const { return player; }

private:
	Start* startPoint;

	Player* player;
};