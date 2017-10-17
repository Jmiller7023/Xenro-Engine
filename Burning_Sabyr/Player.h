#pragma once
#include "Actor.h"
#include <glm\glm.hpp>
#include <XenroEngine\InputManager.h>
#include <XenroEngine\SpriteSheet.h>
#include <XenroEngine\SpriteBatch.h>

class Player :public Actor
{
public:
	Player(float speed, glm::vec2 pos, Xenro::InputManager* inputManager);

	virtual ~Player();
	
	virtual void update(const std::vector<std::string>& WorldData);

	//void draw(Xenro::SpriteBatch& spriteBatch);

private:
	Xenro::InputManager* m_inputManager;
	Xenro::SpriteSheet m_spriteSheet;
	int m_currFrameIndex = 0;
	int m_frameSpeed = 15;
	int m_wait = 0;
};

