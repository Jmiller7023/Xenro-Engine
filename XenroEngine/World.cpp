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
#include "globals.h"
#include "Window.h"

namespace Xenro{

World::World() {

	m_defaultWindowSize = glm::vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	m_scale = glm::vec2(1.0f);

	///Temporary Workaround Delete later!!!
	m_startPlayerPos.x = 128.0f;
	m_startPlayerPos.y = 128.0f;
}

World::World(const std::string filePath) {

	loadLevelData(filePath);
}

void World::loadLevelData(const std::string filePath) {

	//Prevent redrawing old levels with new.
	if (!m_levelData.empty()) {
		m_levelData.clear();
	}

	//Set the spriteBatch to automatically scale.
	if (m_window != nullptr) {
		m_spriteBatch.setWindow(m_window);
		m_spriteBatch.setAutoScale(m_autoResize);
		m_spriteBatch.setDefaultWindowSize(m_defaultWindowSize);
	}

	std::ifstream file;
	file.open(filePath);

	//Error checking.
	if (file.fail()) {
		fatalError("Failed to open " + filePath);
	}

	//Read lines out of the file.
	std::string tmp;
	while (std::getline(file, tmp)) {
		m_levelData.push_back(tmp);
	}

	m_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	ColorRGBA color(255,255,255,255);

	//Render all tiles.
	for (size_t y = 0; y < m_levelData.size(); y++) {
		for (size_t x = 0; x < m_levelData[y].size(); x++) {
			//Grab the tile.
			char tile = m_levelData[y][x];
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			//Process the tile
			std::string path = m_filePaths.getPath(tile);
			if (path != "") {
				m_spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture(m_filePaths.getPath(tile)).ID, 0.0f, color);
			}
		}
	}
	m_spriteBatch.end();

	//Store filePath for later.
	m_currentFilePath = filePath;
}

void World::determineScale() {

	if (m_window->getScreenWidth() != m_currentWindowSize.x || m_window->getScreenHeight() != m_currentWindowSize.y) {
		m_scale.x = (float)m_window->getScreenWidth() / m_defaultWindowSize.x;
		m_scale.y = (float)m_window->getScreenHeight() / m_defaultWindowSize.y;
		loadLevelData(m_currentFilePath);

		//Update current window
		m_currentWindowSize.x = m_window->getScreenWidth();
		m_currentWindowSize.y = m_window->getScreenHeight();
		m_startPlayerPos.x = 128.0f *m_scale.x;
		m_startPlayerPos.y = 128.0f*m_scale.y;
	}
}

World::~World()
{
	//Empty
}

void World::draw() {

	//Rescale tiles if we are autoResizing
	if (m_autoResize) {
		determineScale();
	}

	m_spriteBatch.renderBatch();
}

}