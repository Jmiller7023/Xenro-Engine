#pragma once
#include <glm\glm.hpp>
#include <XenroEngine\Vertex.h>
#include <XenroEngine\SpriteBatch.h>
#include <string>
#include <vector>

const float AGENT_WIDTH = 60;

class Zombie;
class Human;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	void draw(Xenro::SpriteBatch& spriteBatch);

	virtual void update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombie) = 0;

	virtual void collideWithLevel(const std::vector<std::string>& levelData);

	glm::vec2 getPos() const { return m_position; }

protected:
	void checkTilePos(const std::vector<std::string>& levelData, std::vector<glm::vec2>& tilecollisions, float x, float y);

	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 m_position;
	Xenro::ColorRGBA m_color;
	float m_speed;
};

 