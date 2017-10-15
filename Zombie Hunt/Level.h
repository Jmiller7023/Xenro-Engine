#pragma once
#include <string>
#include <vector>
#include <XenroEngine\SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level
{
public:
	Level(const std::string fileName);
	~Level();

	void draw();

	//Getters
	std::vector<std::string>& getLevelData()  { return m_levelData; }
	glm::vec2 getStartPlayerPos() const { return m_startPlayerPos; }
	const std::vector<glm::vec2>& getZombieStartPos() const { return m_zombieStartPos; }
private:
	std::vector<std::string> m_levelData;
	int m_numHumans;
	Xenro::SpriteBatch m_spriteBatch;

	glm::vec2 m_startPlayerPos;
	std::vector<glm::vec2> m_zombieStartPos;
};

