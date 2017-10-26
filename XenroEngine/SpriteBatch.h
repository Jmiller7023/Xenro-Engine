#ifndef XENRO_SPRITEBATCH_DEFINED
#define XENRO_SPRITEBATCH_DEFINED

#include <GL/glew.h>
#include "Vertex.h"
#include "Sprite.h"
#include <glm\glm.hpp>
#include <vector>

namespace Xenro{

	enum class SpriteSortType {
		NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE
};

class RenderBatch {
public:
	RenderBatch(GLuint Offset, GLuint NumVerts, GLuint Texture) 
		:offset(Offset), numVerts(NumVerts), texture(Texture)
	{}
	GLuint offset;
	GLuint numVerts;
	GLuint texture;
};

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	void begin(SpriteSortType sortType = SpriteSortType::TEXTURE);
	void end();

	void draw(const glm::vec4& destRect,const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA& color);
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA& color, float angle);
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA& color, glm::vec2 direction);


	void renderBatch();
private:
	void createRenderBatches();
	void createVertexArray();
	void sortGlyphs();
	float calcAngle(glm::vec2 direction);

	///Comparators.
	static bool compareFrontToBack(Sprite* a, Sprite* b);
	static bool compareBackToFront(Sprite* a, Sprite* b);
	static bool compareTexture(Sprite* a, Sprite* b);

	GLuint m_vbo;
	GLuint m_vao;

	SpriteSortType m_sortType;

	std::vector<Sprite*> m_spritePointers;
	std::vector<Sprite> m_sprites;
	std::vector<RenderBatch> m_renderBatches;
	
};
}

#endif //XENRO_SPRITEBATCH_DEFINED