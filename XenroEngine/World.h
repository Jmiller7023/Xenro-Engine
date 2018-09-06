/*************************************************************************/
/*                              World.h                                  */
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

#ifndef XENRO_WORLD_DEFINED
#define XENRO_WORLD_DEFINED

#include <vector>
#include "SpriteBatch.h"
#include "Globals.h"
#include <string>
#include "FilePathCache.h"

namespace Xenro{

class Window;

class World
{
public:
	World();
	World(const std::string fileName);
	virtual ~World();

	virtual void updateGameWorld() = 0;

	void draw();

	//Getters
	std::vector<std::string>& getLevelData() { return m_levelData; }
	glm::vec2 getStartPlayerPos() const { return m_startPlayerPos; }
	std::string getCurrentFile() const { return m_currentFilePath; }
	glm::vec2 getScale() const { return m_scale; }

	void setAutoResize(bool resizable) { m_autoResize = resizable; }
	void setDefaultWindowSize(glm::vec2 defaultwindowSize) { m_defaultWindowSize = defaultwindowSize; }
	
protected:
	void loadLevelData(std::string filePath);
	Window* m_window;
	Xenro::FilePathCache m_filePaths;

private:
	void determineScale();
	bool m_autoResize = true;
	glm::vec2 m_defaultWindowSize;
	glm::vec2 m_currentWindowSize;
	std::vector<std::string> m_levelData;
	SpriteBatch m_spriteBatch;
	glm::vec2 m_startPlayerPos;
	glm::vec2 m_scale;
	std::string m_currentFilePath;
};

}

#endif //XENRO_WORLD_DEFINED