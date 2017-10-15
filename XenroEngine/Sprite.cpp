#include "Sprite.h"
#include "Vertex.h"
#include <cstddef>
#include "ResourceManager.h"

namespace Xenro{

Sprite::Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA& color)
	:texture(Texture), depth(Depth) 
{

	topLeft.color = color;
	topLeft.setPosition(destRect.x, destRect.y + destRect.w);
	topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	topRight.color = color;
	topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
	topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

	bottomLeft.color = color;
	bottomLeft.setPosition(destRect.x, destRect.y);
	bottomLeft.setUV(uvRect.x, uvRect.y);

	bottomRight.color = color;
	bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
	bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
}

Sprite::Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA& color, float angle)
	:texture(Texture), depth(Depth)
{

	glm::vec2 halfDims(destRect.z / 2.0f, destRect.y / 2.0f);

	//center points at origin.
	glm::vec2 tl(-halfDims.x, halfDims.y);
	glm::vec2 bl(-halfDims.x, -halfDims.y);
	glm::vec2 br(halfDims.x, -halfDims.y);
	glm::vec2 tr(halfDims.x, halfDims.y);

	//Rotate the points.
	tl = rotatePoint(tl, angle) + halfDims;
	bl = rotatePoint(bl, angle) + halfDims;
	br = rotatePoint(br, angle) + halfDims;
	tr = rotatePoint(tr, angle) + halfDims;

	topLeft.color = color;
	topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
	topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	topRight.color = color;
	topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
	topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

	bottomLeft.color = color;
	bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
	bottomLeft.setUV(uvRect.x, uvRect.y);

	bottomRight.color = color;
	bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
	bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
}

glm::vec2 Sprite::rotatePoint(glm::vec2 pos, float angle) {

	glm::vec2 newpos;

	newpos.x = pos.x * cos(angle) - pos.y * sin(angle);
	newpos.y = pos.x * sin(angle) + pos.y * cos(angle);

	return newpos;
}
}