#pragma once
#include "GLTexture.h"
#include <glm\glm.hpp>
namespace Xenro{

class SpriteSheet
{
public:
	SpriteSheet();
	~SpriteSheet();

	void setSpriteSheet(const GLTexture& texture, const glm::ivec2& sheetDims);

	//Getters
	GLTexture getTexture() const { return m_texture; }
	glm::ivec2 getDims() const { return m_sheetDims; }
	glm::vec4 getUVcoords(int i);

private:
	GLTexture m_texture;
	glm::ivec2 m_sheetDims;
};

}