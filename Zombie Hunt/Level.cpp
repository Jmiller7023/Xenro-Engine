#include "Level.h"
#include <XenroEngine\ErrorMessages.h>
#include <fstream>
#include <iostream>
#include <XenroEngine/ResourceManager.h>

Level::Level(const std::string fileName)
{

	std::ifstream file;
	file.open(fileName);

	//Error checking.
	if (file.fail()) {
		Xenro::fatalError("Failed to open " + fileName);
	}

	std::string tmp;
	file >> tmp >> m_numHumans;

	while (std::getline(file, tmp)) {
		m_levelData.push_back(tmp);
	}

	m_spriteBatch.init();
	m_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Xenro::ColorRGBA color;
	color.a = 255;
	color.g = 255;
	color.b = 255;
	color.r = 255;

	//Render all tiles.
	for (size_t y = 0; y < m_levelData.size(); y++) {
		for (size_t x = 0; x < m_levelData[y].size(); x++) {
			//Grab the tile.
			char tile = m_levelData[y][x];

			//Get dest rect.
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			//Process the tile.
			switch (tile) {
			case 'B':
			case 'R':
				m_spriteBatch.draw(destRect, uvRect, Xenro::ResourceManager::getTexture("Textures/red_bricks.png").ID, 0.0f, color);
				break;
			case 'G':
				m_spriteBatch.draw(destRect, uvRect, Xenro::ResourceManager::getTexture("Textures/glass.png").ID, 0.0f, color);
				break;
			case 'L':
				m_spriteBatch.draw(destRect, uvRect, Xenro::ResourceManager::getTexture("Textures/light_bricks.png").ID, 0.0f, color);
				break;
			case '@':
				m_levelData[y][x] = '.';
				m_startPlayerPos.x = x * (float)TILE_WIDTH;
				m_startPlayerPos.y = y * (float)TILE_WIDTH;
				break;
			case 'Z':
				m_levelData[y][x] = '.';
				m_zombieStartPos.emplace_back(x*TILE_WIDTH, y*TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
				break;
			}
		}
	}
	m_spriteBatch.end();
}


Level::~Level()
{
}

void Level::draw() {
	m_spriteBatch.renderBatch();
}