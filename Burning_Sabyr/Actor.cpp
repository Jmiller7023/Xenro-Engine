#include "Actor.h"
#include <algorithm>
#include <XenroEngine\ResourceManager.h>


Actor::Actor()
	:m_frameIndex(0)
{
}


Actor::~Actor()
{
}

void Actor::collideWithWorld(const std::vector<std::string>& worldData) {
	std::vector<glm::vec2> tilecollisions;

	//check the four corners
	//first corner.
	checkTilePos(worldData, tilecollisions, m_position.x, m_position.y);

	//second corner
	checkTilePos(worldData, tilecollisions, m_position.x + AGENT_WIDTH, m_position.y);

	//Third corner
	checkTilePos(worldData, tilecollisions, m_position.x, m_position.y + AGENT_WIDTH);

	//Fourth corner
	checkTilePos(worldData, tilecollisions, m_position.x + AGENT_WIDTH, m_position.y + AGENT_WIDTH);

	for (size_t i = 0; i < tilecollisions.size(); i++) {
		collideWithTile(tilecollisions[i]);
	}
}


void Actor::initializeActor(std::string filePath, Xenro::ColorRGBA color, glm::ivec2 SpriteSheetDims, glm::vec2 Position, glm::vec2 drawDims, glm::vec2 HitboxDims) {
	Xenro::GLTexture texture = Xenro::ResourceManager::getTexture(filePath);
	m_color = color;
	m_position = Position;
	m_drawDims = drawDims;
	m_spriteSheet.setSpriteSheet(texture, SpriteSheetDims);
	m_HitboxDims = HitboxDims;
}

void Actor::draw(Xenro::SpriteBatch& spriteBatch) {

	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = m_drawDims.x;
	destRect.w = m_drawDims.y;

	spriteBatch.draw(destRect, m_spriteSheet.getUVcoords(m_frameIndex), m_spriteSheet.getTexture().ID, 0.0f, m_color);
}

void Actor::checkTilePos(const std::vector<std::string>& levelData, std::vector<glm::vec2>& tilecollisions, float x, float y) {

	glm::vec2 cornerPos1 = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_WIDTH));

	if (levelData[cornerPos1.y][cornerPos1.x] != '.') {
		tilecollisions.push_back(cornerPos1*(float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2));
	}
}

//AABB collision.
void Actor::collideWithTile(glm::vec2 tilePos) {
	const float AGENT_RADIUSX = m_HitboxDims.x / 2.0f;
	const float AGENT_RADIUSY = m_HitboxDims.y / 2.0f;
	const float TILE_RADIUS = (float)AGENT_WIDTH / 2.0f;
	const float MIN_DISTANCEX = AGENT_RADIUSX + TILE_RADIUS;
	const float MIN_DISTANCEY = AGENT_RADIUSY + TILE_RADIUS;

	//divide m_hitboxDim.y by 2 since the sprites are drawn so they start at 0 height.
	glm::vec2 centerPlayerPos = glm::vec2(m_position.x + m_HitboxDims.x, m_position.y + m_HitboxDims.y / 2.0f);
	glm::vec2 distVec = centerPlayerPos - tilePos;

	float xdepth = MIN_DISTANCEX - abs(distVec.x);
	float ydepth = MIN_DISTANCEY - abs(distVec.y);

	if (xdepth > 0 && ydepth > 0) {
		if (std::max(xdepth, 0.0f) < std::max(ydepth, 0.0F)) {
			if (distVec.x < 0) {
				m_position.x -= xdepth;
			}
			else {
				m_position.x += xdepth;
			}
		}
		else {
			if (distVec.y < 0) {
				m_position.y -= ydepth;
			}
			else {
				m_position.y += ydepth;
			}

		}
	}
}
