#pragma once
#include <glm\glm.hpp>
#include <vector>
#include "vertex.h"
#include "GLSLProgram.h"

namespace Xenro{
class OutlineRenderer
{
public:
	OutlineRenderer();
	~OutlineRenderer();

	void end();
	void drawBox(const glm::vec4& destRect, const ColorRGBA& color, float angle);
	void drawCircle(const glm::vec2& center, const ColorRGBA& color, float radius);
	void render(const glm::mat4& Pmat, float lineWidth);

private:
	struct DebugVertex {
		glm::vec2 pos;
		ColorRGBA color;
	};
	GLSLProgram m_program;
	std::vector<DebugVertex> m_vertices;
	std::vector<GLuint> m_indices;
	GLuint m_vbo = 0, m_vao = 0, m_ibo = 0;
	int m_numElements = 0;
};

}