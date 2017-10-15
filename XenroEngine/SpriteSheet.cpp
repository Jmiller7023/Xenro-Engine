#include "SpriteSheet.h"

namespace Xenro{

SpriteSheet::SpriteSheet()
{

}


SpriteSheet::~SpriteSheet()
{

}

void SpriteSheet::setSpriteSheet(const GLTexture& texture, const glm::ivec2& sheetDims) {
	m_texture = texture;
	m_sheetDims = sheetDims;
}

glm::vec4 SpriteSheet::getUVcoords(int i) {
	//This will return the index of the tile needed.
	int xTile = i % m_sheetDims.x;
	int yTile = i / m_sheetDims.x;

	//Creating UV vector.
	glm::vec4 uv;
	uv.x = xTile / (float)m_sheetDims.x;
	uv.y = yTile / (float)m_sheetDims.y;
	uv.z = 1.0f / m_sheetDims.x;
	uv.w = 1.0f / m_sheetDims.y;

	return uv;
}
}