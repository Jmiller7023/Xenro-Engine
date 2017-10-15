#include "LevelLoader.h"



LevelLoader::LevelLoader()
{
}


LevelLoader::~LevelLoader()
{
}

void LevelLoader::updateGameWorld() {
	if (!m_loaded) {
		loadLevelData("Levels/level1.txt");
		m_loaded = true;
	}
}