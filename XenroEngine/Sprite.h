#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "Vertex.h"
#include <string>

namespace Xenro {

	class Sprite {
	public:
		Sprite() {}
		Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA& color);
		Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA& color, float angle);

		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;

	private:
		glm::vec2 rotatePoint(glm::vec2 pos, float angle);
	};

}
