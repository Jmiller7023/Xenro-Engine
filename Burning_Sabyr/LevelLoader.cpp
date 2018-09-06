#include "LevelLoader.h"

LevelLoader::LevelLoader(Xenro::Window* window)
{
	m_window = window;
	m_filePaths.createMapping('R', "Textures/red_bricks.png");
	m_filePaths.createMapping('G', "Textures/glass.png");
	m_filePaths.createMapping('L', "Textures/light_bricks.png");
	m_filePaths.createMapping('.', "Textures/grass.png");
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