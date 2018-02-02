/*************************************************************************/
/*                         SpriteBatch.cpp                               */
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

#include "SpriteBatch.h"
#include <algorithm>

namespace Xenro{

SpriteBatch::SpriteBatch()
	:m_vbo(0), m_vao(0)
{
	createVertexArray();
}


SpriteBatch::~SpriteBatch()
{
	if (m_vao != 0) {
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
	if (m_vbo != 0) {
		glDeleteBuffers(1, &m_vbo);
		m_vbo = 0;
	}
}

void SpriteBatch::begin(SpriteSortType sortType) {

	m_sortType = sortType;

	m_renderBatches.clear();
	m_sprites.clear();
}

void SpriteBatch::end() {

	//set up all pointers for faster sorting.
	m_spritePointers.resize(m_sprites.size());
	for (size_t i = 0; i < m_sprites.size(); i++) {
		m_spritePointers[i] = &m_sprites[i];
	}

	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA& color) {

	m_sprites.emplace_back(destRect, uvRect, texture, depth, color);
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA& color, float angle) {

	m_sprites.emplace_back(destRect, uvRect, texture, depth, color, angle);
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA& color, glm::vec2 direction) {

	float angle = calcAngle(direction);
	m_sprites.emplace_back(destRect, uvRect, texture, depth, color, angle);
}

void SpriteBatch::renderBatch() {

	//bind vertex array
	glBindVertexArray(m_vao);

	for (size_t i = 0; i < m_renderBatches.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);

		glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVerts);
	}
	//unbind vertex array
	glBindVertexArray(0);
}

void SpriteBatch::createRenderBatches() {

	std::vector<Vertex> vertices;
	vertices.resize(m_spritePointers.size() * 6);

	if (m_spritePointers.empty()) {
		return;
	}

	int offset = 0;
	int currentVertex = 0;

	//constructs our first render batch and pushes it back in the vector.
	m_renderBatches.emplace_back(offset, 6, m_spritePointers[0]->texture);
	vertices[currentVertex++] = m_spritePointers[0]->topLeft;
	vertices[currentVertex++] = m_spritePointers[0]->bottomLeft;
	vertices[currentVertex++] = m_spritePointers[0]->bottomRight;
	vertices[currentVertex++] = m_spritePointers[0]->bottomRight;
	vertices[currentVertex++] = m_spritePointers[0]->topRight;
	vertices[currentVertex++] = m_spritePointers[0]->topLeft;
	offset += 6;

	for (size_t cs = 1; cs < m_spritePointers.size(); cs++) {
		if(m_spritePointers[cs]->texture != m_spritePointers[cs - 1]->texture){
		m_renderBatches.emplace_back(offset, 6, m_spritePointers[cs]->texture);
		}
		else {
			m_renderBatches.back().numVerts += 6;
		}
		vertices[currentVertex++] = m_spritePointers[cs]->topLeft;
		vertices[currentVertex++] = m_spritePointers[cs]->bottomLeft;
		vertices[currentVertex++] = m_spritePointers[cs]->bottomRight;
		vertices[currentVertex++] = m_spritePointers[cs]->bottomRight;
		vertices[currentVertex++] = m_spritePointers[cs]->topRight;
		vertices[currentVertex++] = m_spritePointers[cs]->topLeft;
		offset += 6;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//orphan the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	//upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	//unbind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
void SpriteBatch::createVertexArray() {

	if(m_vao == 0){
		glGenVertexArrays(1, &m_vao);
	}
	// Bind the VAO. All subsequent opengl calls will modify it's state.
	glBindVertexArray(m_vao);

	if (m_vbo == 0) {
		glGenBuffers(1, &m_vbo);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	//Tell opengl that we want to use the first
	//attribute array. We only need one array right
	//now since we are only using position.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//this is position attribute pointer.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	//This is the color attribute pointer.
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	//This is the UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}

void SpriteBatch::sortGlyphs() {
	switch (m_sortType) {
	case SpriteSortType::BACK_TO_FRONT:
	    std::stable_sort(m_spritePointers.begin(), m_spritePointers.end(), compareBackToFront);
		break;
	case SpriteSortType::FRONT_TO_BACK:
		std::stable_sort(m_spritePointers.begin(), m_spritePointers.end(), compareFrontToBack);
		break;
	case SpriteSortType::TEXTURE:
		std::stable_sort(m_spritePointers.begin(), m_spritePointers.end(), compareTexture);
		break;

	}

}

bool SpriteBatch::compareFrontToBack(Sprite* a, Sprite* b) {
	return (a->depth < b->depth);
 }

bool SpriteBatch::compareBackToFront(Sprite* a, Sprite* b) {
	return (a->depth > b->depth);
 }

 bool SpriteBatch::compareTexture(Sprite* a, Sprite* b) {
	 return (a->texture < b->texture);
}

 float SpriteBatch::calcAngle(glm::vec2 direction) {
	 float angle = acos(glm::dot(glm::vec2(1.0f, 0.0f), direction));
	 if (direction.y < 0) {
		 angle = -angle;
	 }
	 return angle;
 }


}