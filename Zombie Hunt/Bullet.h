#pragma once
#include <glm\glm.hpp>
#include <XenroEngine\SpriteBatch.h>
class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();

	void draw(Xenro::SpriteBatch& spriteBatch);
	bool update();

private:
	int m_lifeTime;
	float m_speed;
	glm::vec2 m_direction;
	glm::vec2 m_position;
};

