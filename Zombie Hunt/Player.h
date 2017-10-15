#pragma once
#include <XenroEngine\Actors.h>
#include <glm\glm.hpp>
#include <XenroEngine\InputManager.h>
#include <XenroEngine\SpriteSheet.h>

class Player :public Xenro::Actors
{
public:
	Player();
	virtual ~Player();
	
	void init(float speed, glm::vec2 pos, Xenro::InputManager* inputManager);

	virtual void update(const std::vector<std::string>& WorldData);

private:
	glm::vec2 m_DrawDims;
	Xenro::InputManager* m_inputManager;
	Xenro::SpriteSheet m_spriteSheet;

};

