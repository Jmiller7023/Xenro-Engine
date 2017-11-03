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

namespace Xenro{

class Camera;
class InputManager;

class World
{
public:
	World() {}
	World(const std::string fileName);
	~World();

	virtual void updateGameWorld() = 0;

	void draw();

	//Getters
	std::vector<std::string>& getLevelData() { return m_levelData; }
	glm::vec2 getStartPlayerPos() const { return m_startPlayerPos; }
	
protected:
	Camera* getGameCam() const { return m_camera; }
	InputManager* getInputManager() const { return m_inputManager; }
	void loadLevelData(std::string filePath);

private:
	Camera* m_camera;
	InputManager* m_inputManager;
	std::vector<std::string> m_levelData;
	int m_numHumans;
	Xenro::SpriteBatch m_spriteBatch;

	glm::vec2 m_startPlayerPos;
	std::vector<glm::vec2> m_zombieStartPos;
};

}

#endif //XENRO_WORLD_DEFINED