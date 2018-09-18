/*************************************************************************/
/*                         OutlineRenderer.h                             */
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

#ifndef XENRO_OUTLINERENDERER_DEFINED
#define XENRO_OUTLINERENDERER_DEFINED

#include <glm\glm.hpp>
#include <vector>
#include "vertex.h"
#include "GLSLProgram.h"

namespace Xenro{
class Window;

class OutlineRenderer {
public:
	OutlineRenderer();
	OutlineRenderer(Window* window, bool autoScale, glm::vec2 defaultWindowSize);
	~OutlineRenderer();

	void end();
	void drawLine(const glm::vec2& a, const glm::vec2& b, const ColorRGBA& color);
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

#endif //XENRO_OUTLINERENDERER_DEFINED