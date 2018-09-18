/*************************************************************************/
/*                           SpriteBatch.h                               */
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

#ifndef XENRO_SPRITEBATCH_DEFINED
#define XENRO_SPRITEBATCH_DEFINED

#include <GL/glew.h>
#include "Vertex.h"
#include "Sprite.h"
#include <glm\glm.hpp>
#include <vector>

namespace Xenro{

class Window;

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
	//Ordinary spriteBatch with automatic scaling disabled.
	SpriteBatch();
	//Spritebatch with the ability to automatically scale all sprites regardless of window size.
	SpriteBatch(Window* window, bool autoScale, glm::vec2 defaultWindowSize);
	~SpriteBatch();

	void begin(SpriteSortType sortType = SpriteSortType::TEXTURE);
	void end();

	void draw(const glm::vec4& destRect,const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA& color);
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA& color, float angle);
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA& color, glm::vec2 direction);

	void renderBatch();
	///Getters
	glm::vec2 getDefaultWindowSize() const { return m_defaultWindowSize; }
	glm::vec2 getScale() const { return m_scale; }

	///Setters
	//Have the sprites automatically scale based off of the default window size.
	void setAutoScale(bool autoScale) { m_autoScale = autoScale; }
	//Give the spritebatch access to the window it will be drawing to for access to scaling.
	void setWindow(Window* window) { m_window = window; }
	//Determine the value of the window size everything should scale to. By default it is 1920x1080.
	void setDefaultWindowSize(glm::vec2 defaultWindowSize) { m_defaultWindowSize = defaultWindowSize; }
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

	//Screen Scaling
	void determineScale();
	glm::vec4 applyScale(const glm::vec4& destRect) const;
	glm::vec2 m_scale;
	glm::vec2 m_defaultWindowSize;
	glm::vec2 m_currentWindowSize;
	Window* m_window;
	bool m_autoScale;

};
}

#endif //XENRO_SPRITEBATCH_DEFINED