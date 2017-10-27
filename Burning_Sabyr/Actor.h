#ifndef ACTOR_DEFINED
#define ACTOR_DEFINED

#include <vector>
#include <glm\glm.hpp>
#include <XenroEngine\SpriteBatch.h>
#include <string>
#include <XenroEngine\SpriteSheet.h>
#include <XenroEngine\GLTexture.h>
#include <XenroEngine\Globals.h>
#include <XenroEngine\Vertex.h>

enum class MoveDir {
	UP, DOWN, LEFT, RIGHT, UPRIGHT, UPLEFT, DOWNRIGHT, DOWNLEFT, IDLE
};

class Actor
{
public:
	Actor();
	~Actor();

	virtual void update(const std::vector<std::string>& WorldData) { }
	virtual void update() {}

	void draw(Xenro::SpriteBatch& spriteBatch);

	void initializeActor(std::string filePath, Xenro::ColorRGBA color, glm::ivec2 SpriteSheetDims, glm::vec2 Position, glm::vec2 drawDims, glm::vec2 HitboxDims);

	virtual void collideWithWorld(const std::vector<std::string>& worldData);

	//Getters
	glm::vec2 getPos() const {return m_position; }
	glm::vec2 getDrawDims() const { return m_drawDims; }
	glm::vec2 getHboxDims() const { return m_HitboxDims; }

protected:
	void checkTilePos(const std::vector<std::string>& levelData, std::vector<glm::vec2>& tilecollisions, float x, float y);

	void collideWithTile(glm::vec2 tilePos);

	int m_frameIndex;
	
	void setFrameIndex(int i) { m_frameIndex = i; }

	void setMoveDir(MoveDir dir) { m_moveDir = dir; }

	glm::vec2 m_position;
	glm::vec2 m_direction = glm::vec2(1.0f, 0.0f);
	Xenro::ColorRGBA m_color;
	float m_speed;
	Xenro::SpriteSheet m_spriteSheet;
	glm::vec2 m_drawDims;
	glm::vec2 m_HitboxDims;
	MoveDir m_moveDir = MoveDir::IDLE;
};

#endif //ACTOR_DEFINED