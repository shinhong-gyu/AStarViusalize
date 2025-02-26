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
	// 맵 grid
	std::vector<std::vector<int> > grid;

	// 경로를 따라 움직임
	void MoveAlongPath();
private:
	// AStar 객체
	AStar* pathFinder;

	// A*알고리즘을 통해 구해진 경로
	std::vector<Node*> path;

	// 플레이어의 움직임을 제어하는 변수
	bool bOnMove = false;

	// 움직인 횟수를 카운트해서 path의 경로를 모두 방문했다면 움직이지 않도록 하기 위한 변수
	int moveCount = 0;

	// 움직임에 지연을 주기 위한 Timer 객체
    Timer* timer = new Timer(0.1f);

	// Tab이 입력되면 게임을 일정 시간 이후에 종료시키기 위한 Timer 객체
    Timer* quitTimer= new Timer(3.0f);

	// demoLevel 객체
    static DemoLevel* demoLevel;

	// 자취를 기록하기 위한 한 스탭 이전의 위치
    Vector2 prePosition;

	// 화면 크기 안에서 움직이도록 제어하기 위한 변수
	int screenWidth = 0;
	int screenHeight = 0;

	// 콘솔 화면을 플레이어와 같이 움직이도록 하기 위한 변수
	Vector2 curWindowPos = 0;

	// Tab이 눌렸는지 판별하기 위한 변수
	bool bToggle = false;
};