/*
#include "GameWorld.h"
#include <glm\glm.hpp>
#include <XenroEngine\ResourceManager.h>


GameWorld::GameWorld(Xenro::Game& game)
{
	m_game = game;
	m_inputManager = game.getInputManager();
	m_camera = game.getCamera();
}


GameWorld::~GameWorld()
{
}

void GameWorld::updateGameWorld() {

	for (size_t i = 0; i < m_bullets.size();) {
		if (m_bullets[i].update() == true) {
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	if (m_inputManager->isDown(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = m_inputManager->getMouseCoords();
		mouseCoords = m_camera->convertScreentoWorld(mouseCoords);
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		m_bullets.emplace_back(playerPosition, direction, 3.00f, 1000);
	}

	m_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Xenro::GLTexture texture = Xenro::ResourceManager::getTexture("Textures/Cleric.png");
	Xenro::ColorRGBA color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	m_spriteBatch.draw(pos, uv, texture.ID, 0.0f, color);

	for (size_t i = 0; i < m_bullets.size(); i++) {
		m_bullets[i].draw(m_spriteBatch);
	}
	m_spriteBatch.end();

	m_spriteBatch.renderBatch();
}*/