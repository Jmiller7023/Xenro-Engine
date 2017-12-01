/*************************************************************************/
/*                            World.cpp                                  */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Xenro Engine                                */
/*             https://github.com/Jmiller7023/Xenro-Engine               */
/*************************************************************************/
/*                 Copyright  11-3-2017 Joseph Miller.                   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "World.h"
#include "ErrorMessages.h"
#include <fstream>
#include <iostream>
#include "ResourceManager.h"

namespace Xenro{

World::World(const std::string filePath)
{
	loadLevelData(filePath);
}

void World::loadLevelData(const std::string filePath) {

	std::ifstream file;
	file.open(filePath);

	//Error checking.
	if (file.fail()) {
		fatalError("Failed to open " + filePath);
	}

	std::string tmp;
	file >> tmp >> m_numHumans;

	while (std::getline(file, tmp)) {
		m_levelData.push_back(tmp);
	}

	m_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	ColorRGBA color;
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
				m_spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture("Textures/red_bricks.png").ID, 0.0f, color);
				break;
			case 'G':
				m_spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture("Textures/glass.png").ID, 0.0f, color);
				break;
			case 'L':
				m_spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture("Textures/light_bricks.png").ID, 0.0f, color);
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

World::~World()
{
	//Empty
}

void World::draw() {
	m_spriteBatch.renderBatch();
}

}