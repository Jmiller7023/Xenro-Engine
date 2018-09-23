#include "Tile.h"

Xenro::Tile::Tile()
{
}

Xenro::Tile::Tile(b2World * world, const glm::vec2 & position, const glm::vec2 & dimensions, GLTexture texture, ColorRGBA color, bool fixedRotation, bool isDynamic, float angle, glm::vec4 uvRect)
{
	init(world, position, dimensions, texture, color, fixedRotation, isDynamic, angle, uvRect);
}

Xenro::Tile::~Tile()
{
	destroy(m_world);
}

void Xenro::Tile::init(b2World * world, const glm::vec2 & position, const glm::vec2 & dimensions, GLTexture texture, ColorRGBA color, bool fixedRotation, bool isDynamic, float angle, glm::vec4 uvRect)
{
	m_dimensions = dimensions;
	m_color = color;
	m_texture = texture;
	m_uvRect = uvRect;
	m_fixedRotation = fixedRotation;
	m_isDynamic = isDynamic;

	// Make the body
	b2BodyDef bodyDef;
	if (isDynamic) {
		bodyDef.type = b2_dynamicBody;
	}
	else {
		bodyDef.type = b2_staticBody;
	}
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = fixedRotation;
	bodyDef.angle = angle;
	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);
}

void Xenro::Tile::destroy(b2World * world)
{
	if (world != nullptr) {
		world->DestroyBody(m_body);
	}
}

void Xenro::Tile::draw(SpriteBatch & spriteBatch)
{
	glm::vec4 destRect;
	destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
	destRect.y = m_body->GetPosition().y - m_dimensions.y / 2.0f;
	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;
	spriteBatch.draw(destRect, m_uvRect, m_texture.ID, 0.0f, m_color, m_body->GetAngle());
}
