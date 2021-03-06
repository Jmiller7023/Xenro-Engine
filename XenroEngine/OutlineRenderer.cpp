/*************************************************************************/
/*                        OutlineRenderer.cpp                            */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Xenro Engine                                */
/*             https://github.com/Jmiller7023/Xenro-Engine               */
/*************************************************************************/
/*                 Copyright  11-3-2017 Joseph Miller.                   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "OutlineRenderer.h"
#include "Window.h"
#include "Globals.h"

namespace Xenro {

//the namespace here prevents name collisions in other files.
namespace {
		const char* VERT_SRC = R"(#version 130
//The vertex shader operates on each vertex
//input data from the VBO. Each vertex is 2 floats
in vec2 vertexPosition;
in vec4 vertexColor;
out vec2 fragmentPosition;
out vec4 fragmentColor;
uniform mat4 P;
void main() {
    //Set the x,y position on the screen
    gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
    //the z position is zero since we are in 2D
    gl_Position.z = 0.0;
    
    //Indicate that the coordinates are normalized
    gl_Position.w = 1.0;
    
    fragmentPosition = vertexPosition;
    
    fragmentColor = vertexColor;
})";

		const char* FRAG_SRC = R"(#version 130
//The fragment shader operates on each pixel in a given polygon
in vec2 fragmentPosition;
in vec4 fragmentColor;
//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;
void main() {
    color = fragmentColor;
})";
	}

const float pi = 3.14159265358979323f;

OutlineRenderer::OutlineRenderer()
	:m_window(nullptr), m_autoScale(false)
{
	m_defaultWindowSize = glm::vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	m_scale = glm::vec2(1.0f);

	// Shader init
	m_program.compileShadersFromSource(VERT_SRC, FRAG_SRC);
	m_program.addAttribute("vertexPosition");
	m_program.addAttribute("vertexColor");
	m_program.linkShaders();

	// Set up buffers
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, pos));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

	glBindVertexArray(0);
}


OutlineRenderer::OutlineRenderer(Window * window, bool autoScale, glm::vec2 defaultWindowSize)
	:m_window(window), m_autoScale(autoScale), m_defaultWindowSize(defaultWindowSize)
{
	m_scale = glm::vec2(1.0f);

	// Shader init
	m_program.compileShadersFromSource(VERT_SRC, FRAG_SRC);
	m_program.addAttribute("vertexPosition");
	m_program.addAttribute("vertexColor");
	m_program.linkShaders();

	// Set up buffers
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, pos));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

	glBindVertexArray(0);
}

OutlineRenderer::~OutlineRenderer()
{
	if (m_vao != 0) {
		glDeleteVertexArrays(1, &m_vao);
	}
	if (m_vbo != 0) {
		glDeleteBuffers(1, &m_vbo);
	}
	if (m_ibo != 0) {
		glDeleteBuffers(1, &m_ibo);
	}
}

void OutlineRenderer::end() {
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//Orphan the buffer.
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(DebugVertex), nullptr,  GL_DYNAMIC_DRAW);
	//upload the data.
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(DebugVertex), m_vertices.data() );
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	//Orphan the buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
	//upload the data.
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(GLuint), m_indices.data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_numElements = m_indices.size();
	m_indices.clear();
	m_vertices.clear();
}

void OutlineRenderer::drawLine(const glm::vec2 & a, const glm::vec2 & b, const ColorRGBA & color)
{
	int i = m_vertices.size();
	m_vertices.resize(m_vertices.size() + 2);

	m_vertices[i].pos = a;
	m_vertices[i].color = color;
	m_vertices[i + 1].pos = b;
	m_vertices[i + 1].color = color;

	m_indices.push_back(i);
	m_indices.push_back(i + 1);
}

glm::vec2 rotatePoint(glm::vec2 pos, float angle) {
	glm::vec2 newpos;

	newpos.x = pos.x * cos(angle) - pos.y * sin(angle);
	newpos.y = pos.x * sin(angle) + pos.y * cos(angle);

	return newpos;
}

void OutlineRenderer::drawBox(const glm::vec4& destRect, const ColorRGBA& color, float angle) {

	int i = m_vertices.size();
	m_vertices.resize(m_vertices.size() + 4);
	glm::vec4 rect;
	if (m_autoScale && m_window != nullptr) {
		determineScale();
		rect = applyScale(destRect);
	}
	else {
		rect = destRect;
	}

	glm::vec2 halfDims(rect.z / 2.0f, rect.w / 2.0f);

	//center points at origin.
	glm::vec2 tl(-halfDims.x, halfDims.y);
	glm::vec2 bl(-halfDims.x, -halfDims.y);
	glm::vec2 br(halfDims.x, -halfDims.y);
	glm::vec2 tr(halfDims.x, halfDims.y);

	glm::vec2 positionOffset(rect.x, rect.y);

	//Rotate the points.
	m_vertices[i].pos = rotatePoint(tl, angle) + halfDims + positionOffset;
	m_vertices[i + 1].pos = rotatePoint(bl, angle) + halfDims + positionOffset;
	m_vertices[i + 2].pos = rotatePoint(br, angle) + halfDims + positionOffset;
	m_vertices[i + 3].pos = rotatePoint(tr, angle) + halfDims + positionOffset;

	for (int j = i; j < i + 4; j++) {
		m_vertices[j].color = color;
	}

	m_indices.reserve(m_indices.size() + 8);

	m_indices.push_back(i);
	m_indices.push_back(i + 1);

	m_indices.push_back(i + 1);
	m_indices.push_back(i + 2);

	m_indices.push_back(i + 2);
	m_indices.push_back(i + 3);

	m_indices.push_back(i + 3);
	m_indices.push_back(i);

}

void OutlineRenderer::drawCircle(const glm::vec2& center, const ColorRGBA& color, float radius) {
	static const int NUM_VERTICES = 100;
	//Set up Vertices.
	int start = m_vertices.size();
	m_vertices.resize(m_vertices.size() + NUM_VERTICES);
	for (int i = 0; i < NUM_VERTICES; i++) {
		float angle = ((float)i / NUM_VERTICES) * pi * 2.0f;
		m_vertices[start + i].pos.x = center.x + cos(angle) * radius;
		m_vertices[start + i].pos.y = center.y + sin(angle) * radius;
		m_vertices[start + i].color = color;
	}
	//Set up indices for indexed drawing.
	m_indices.reserve(m_indices.size() + NUM_VERTICES * 2);
	for (int i = 0; i < NUM_VERTICES - 1; i++) {
		m_indices.push_back(start + i);
		m_indices.push_back(start + i + 1);
	}
	m_indices.push_back(start + NUM_VERTICES - 1);
	m_indices.push_back(start);
}

void OutlineRenderer::render(const glm::mat4& Pmat, float lineWidth) {
	m_program.use();

	//Set the camera matrix.
	GLint pLocation = m_program.getUniformLocation("P");
	//Pass pointer to openGL
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(Pmat[0][0]));
	glLineWidth(lineWidth);

	glBindVertexArray(m_vao);
	glDrawElements(GL_LINES, m_numElements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	m_program.unuse();
}

void OutlineRenderer::determineScale() {

	if (m_window->getScreenWidth() != m_currentWindowSize.x || m_window->getScreenHeight() != m_currentWindowSize.y) {
		m_scale = glm::vec2((float)m_window->getScreenWidth() / m_defaultWindowSize.x,
			(float)m_window->getScreenHeight() / m_defaultWindowSize.y);

		//Update current window
		m_currentWindowSize = glm::vec2(m_window->getScreenWidth(), m_window->getScreenHeight());
	}
}

glm::vec4 OutlineRenderer::applyScale(const glm::vec4 & destRect) const {

	float x = destRect.x * m_scale.x;
	float y = destRect.y * m_scale.y;
	float z = destRect.z * m_scale.x;
	float w = destRect.w * m_scale.y;
	return glm::vec4(x, y, z, w);
}



}