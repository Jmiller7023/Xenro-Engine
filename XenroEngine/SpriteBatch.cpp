#include "SpriteBatch.h"
#include <algorithm>

namespace Xenro{


Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA& color)
	:texture(Texture), depth(Depth) {

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

Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA& color, float angle)
	:texture(Texture), depth(Depth) {

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

glm::vec2 Glyph::rotatePoint(glm::vec2 pos, float angle) {
	glm::vec2 newpos;

	newpos.x = pos.x * cos(angle) - pos.y * sin(angle);
	newpos.y = pos.x * sin(angle) + pos.y * cos(angle);
	
	return newpos;
}

SpriteBatch::SpriteBatch()
	:m_vbo(0), m_vao(0)
{
	createVertexArray();
}


SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::begin(GlyphSortType sortType) {
	m_sortType = sortType;

	m_renderBatches.clear();
	m_glyphs.clear();
}

void SpriteBatch::end() {
	//set up all pointers for faster sorting.
	m_glyphpointers.resize(m_glyphs.size());
	for (size_t i = 0; i < m_glyphs.size(); i++) {
		m_glyphpointers[i] = &m_glyphs[i];
	}

	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA& color) {

	m_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA& color, float angle) {

	m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA& color, glm::vec2 direction) {
	float angle = calcAngle(direction);

	m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
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
	vertices.resize(m_glyphpointers.size() * 6);

	if (m_glyphpointers.empty()) {
		return;
	}
	int offset = 0;
	int currentVertex = 0;

	//constructs our first render batch and pushes it back in the vector.
	m_renderBatches.emplace_back(offset, 6, m_glyphpointers[0]->texture);
	vertices[currentVertex++] = m_glyphpointers[0]->topLeft;
	vertices[currentVertex++] = m_glyphpointers[0]->bottomLeft;
	vertices[currentVertex++] = m_glyphpointers[0]->bottomRight;
	vertices[currentVertex++] = m_glyphpointers[0]->bottomRight;
	vertices[currentVertex++] = m_glyphpointers[0]->topRight;
	vertices[currentVertex++] = m_glyphpointers[0]->topLeft;
	offset += 6;
	int cg = 1; //current glyph

	for (int cg = 1; cg < m_glyphpointers.size(); cg++) {
		if(m_glyphpointers[cg]->texture != m_glyphpointers[cg - 1]->texture){
		m_renderBatches.emplace_back(offset, 6, m_glyphpointers[cg]->texture);
		}
		else {
			m_renderBatches.back().numVerts += 6;
		}
		vertices[currentVertex++] = m_glyphpointers[cg]->topLeft;
		vertices[currentVertex++] = m_glyphpointers[cg]->bottomLeft;
		vertices[currentVertex++] = m_glyphpointers[cg]->bottomRight;
		vertices[currentVertex++] = m_glyphpointers[cg]->bottomRight;
		vertices[currentVertex++] = m_glyphpointers[cg]->topRight;
		vertices[currentVertex++] = m_glyphpointers[cg]->topLeft;
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
	case GlyphSortType::BACK_TO_FRONT:
	    std::stable_sort(m_glyphpointers.begin(), m_glyphpointers.end(), compareBackToFront);
		break;
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(m_glyphpointers.begin(), m_glyphpointers.end(), compareFrontToBack);
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(m_glyphpointers.begin(), m_glyphpointers.end(), compareTexture);
		break;

	}

}

bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
	return (a->depth < b->depth);
 }

bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
	return (a->depth > b->depth);
 }

 bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
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