#include "DemoLevel.h"
#include "Actor/Player.h"
#include "Actor/Start.h"


DemoLevel::DemoLevel()
{
	startPoint = new Start();
	AddActor(startPoint);
	AddActor(new Player());
}

DemoLevel::~DemoLevel()
{
}

void DemoLevel::Draw()
{
	Super::Draw();
}

Start* DemoLevel::GetStartPoint()
{
	return startPoint;
}
