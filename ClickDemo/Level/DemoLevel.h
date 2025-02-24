#pragma once

#include "Level/Level.h"
#include <vector>

class Start;

class DemoLevel : public Level
{
	// RTTI ����.
	RTTI_DECLARATIONS(DemoLevel, Level)

public:
	DemoLevel();
	~DemoLevel();
	virtual void Draw() override;

	Start* GetStartPoint();


private:
	Start* startPoint;
};