#pragma once
#include <XenroEngine\World.h>
class LevelLoader : public Xenro::World
{
public:
	LevelLoader();
	~LevelLoader();

	virtual void updateGameWorld();

private:
	bool m_loaded = false;
};

