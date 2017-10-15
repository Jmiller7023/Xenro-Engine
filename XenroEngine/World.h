#pragma once
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