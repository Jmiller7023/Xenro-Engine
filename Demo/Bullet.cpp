#include "Bullet.h"
#include <XenroEngine\ResourceManager.h>


Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime)
	:m_position(pos), m_direction(dir), m_speed(speed), m_lifeTime(lifeTime)
{
	//Empty
}


Bullet::~Bullet()
{
	//Empty
}


void Bullet::draw(Xenro::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Xenro::GLTexture texture = Xenro::ResourceManager::getTexture("Textures/Circle.png");
	Xenro::ColorRGBA color(20,20,20,255);

	glm::vec4 posAndSize = glm::vec4(m_position.x + 30, m_position.y + 20, 20, 20);

	spriteBatch.draw(posAndSize, uv, texture.ID, -1.0f, color);
}

void Bullet::updateActors() 
{
	//Empty
}

bool Bullet::updatePos() {

	m_position += m_direction * m_speed;
	m_lifeTime--;
 
	if (m_lifeTime == 0) {
		return true;
	}
	return false;
}