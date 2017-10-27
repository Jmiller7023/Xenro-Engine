#include "LevelLoader.h"

LevelLoader::LevelLoader()
{
	//Empty
}


LevelLoader::~LevelLoader()
{
	//Empty
}

void LevelLoader::updateGameWorld() {

	if (!m_loaded) {
		loadLevelData("Levels/level1.txt");
		m_loaded = true;
	}
}