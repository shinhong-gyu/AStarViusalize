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
	// �� grid
	std::vector<std::vector<int> > grid;

	// ��θ� ���� ������
	void MoveAlongPath();
private:
	// AStar ��ü
	AStar* pathFinder;

	// A*�˰����� ���� ������ ���
	std::vector<Node*> path;

	// �÷��̾��� �������� �����ϴ� ����
	bool bOnMove = false;

	// ������ Ƚ���� ī��Ʈ�ؼ� path�� ��θ� ��� �湮�ߴٸ� �������� �ʵ��� �ϱ� ���� ����
	int moveCount = 0;

	// �����ӿ� ������ �ֱ� ���� Timer ��ü
    Timer* timer = new Timer(0.1f);

	// Tab�� �ԷµǸ� ������ ���� �ð� ���Ŀ� �����Ű�� ���� Timer ��ü
    Timer* quitTimer= new Timer(3.0f);

	// demoLevel ��ü
    static DemoLevel* demoLevel;

	// ���븦 ����ϱ� ���� �� ���� ������ ��ġ
    Vector2 prePosition;

	// ȭ�� ũ�� �ȿ��� �����̵��� �����ϱ� ���� ����
	int screenWidth = 0;
	int screenHeight = 0;

	// �ܼ� ȭ���� �÷��̾�� ���� �����̵��� �ϱ� ���� ����
	Vector2 curWindowPos = 0;

	// Tab�� ���ȴ��� �Ǻ��ϱ� ���� ����
	bool bToggle = false;
};