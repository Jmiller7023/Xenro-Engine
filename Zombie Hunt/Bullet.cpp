#include "Bullet.h"
#include <XenroEngine\ResourceManager.h>


Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime)
	:m_position(pos), m_direction(dir), m_speed(speed), m_lifeTime(lifeTime)
{
}


Bullet::~Bullet()
{
}


void Bullet::draw(Xenro::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Xenro::GLTexture texture = Xenro::ResourceManager::getTexture("Textures/glass.png");
	Xenro::ColorRGBA color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	glm::vec4 posAndSize = glm::vec4(m_position.x, m_position.y, 30, 30);

	spriteBatch.draw(posAndSize, uv, texture.ID, 0.0f, color);
}

bool Bullet::update() {
	m_position += m_direction * m_speed;
	m_lifeTime--;
	if (m_lifeTime < 0) {
		return true;
	}
	return false;
}