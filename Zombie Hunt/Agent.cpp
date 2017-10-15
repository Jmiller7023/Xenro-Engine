#include "Agent.h"
#include <XenroEngine\ResourceManager.h>
#include "Level.h"
#include <algorithm>
Agent::Agent()
{
}


Agent::~Agent()
{
}

void Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> tilecollisions;

	//check the four corners
	//first corner.
	checkTilePos(levelData, tilecollisions, m_position.x, m_position.y);

	//second corner
	checkTilePos(levelData, tilecollisions, m_position.x + AGENT_WIDTH, m_position.y);

	//Third corner
	checkTilePos(levelData, tilecollisions, m_position.x, m_position.y + AGENT_WIDTH);

	//Fourth corner
	checkTilePos(levelData, tilecollisions, m_position.x + AGENT_WIDTH, m_position.y + AGENT_WIDTH);

	for (size_t i = 0; i < tilecollisions.size(); i++) {
		collideWithTile(tilecollisions[i]);
	}
}

void Agent::draw(Xenro::SpriteBatch& spriteBatch) {
	static int textureID = Xenro::ResourceManager::getTexture("Textures/circle.png").ID;

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	spriteBatch.draw(destRect, uvRect, textureID,0.0f, m_color);
}

void Agent::checkTilePos(const std::vector<std::string>& levelData, std::vector<glm::vec2>& tilecollisions, float x, float y) {
	
	glm::vec2 cornerPos1 = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_WIDTH));

	if (levelData[cornerPos1.y][cornerPos1.x] != '.') {
		tilecollisions.push_back(cornerPos1*(float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2));
	}
}

//AABB collision.
void Agent::collideWithTile(glm::vec2 tilePos) {
	const float AGENT_RADIUS = (float)AGENT_WIDTH / 2.0f;
	const float TILE_RADIUS = (float)AGENT_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = m_position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPos - tilePos;

	float xdepth = MIN_DISTANCE - abs(distVec.x);
	float ydepth = MIN_DISTANCE - abs(distVec.y);

	if (xdepth > 0 && ydepth > 0) {
		if (std::max(xdepth, 0.0f) < std::max(ydepth, 0.0F)) {
			if (distVec.x < 0) {
				m_position.x -= xdepth;
			}
			else {
				m_position.x += xdepth;
			}
		}else {
			if (distVec.y < 0) {
				m_position.y -= ydepth;
			}
			else {
				m_position.y += ydepth;
			}
		}
	}
}