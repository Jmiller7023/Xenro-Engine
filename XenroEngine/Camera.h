/*************************************************************************/
/*                              Camera.h                                 */
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

#ifndef XENRO_CAMERA_DEFINED
#define XENRO_CAMERA_DEFINED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace Xenro {

class Window;
class GLSLProgram;

class Camera {
public:
	Camera();
	Camera(Window* window);
	~Camera();

	void reset(Window* window);

	void update();

	glm::vec2 convertScreentoWorld(glm::vec2 screenCoords);

	bool isInCam(const glm::vec2& position,const glm::vec2& dimensions);

	void updateUniform(GLSLProgram* textureProgram, const std::string& uniform);

	void offsetPosition(const glm::vec2& offset) { m_position += offset; m_needsUpdate = true; }
	void offsetScale(float offset) { m_scale = ((offset > 0.001) ? (offset + m_scale) : 0.001f); m_needsUpdate = true; }

	//setters
	void setPosition(const glm::vec2& newPosition) { m_position = newPosition; m_needsUpdate = true; }
	void setScale(float newScale) { m_scale = newScale; m_needsUpdate = true; }
	void setNeedsUpdate() { m_needsUpdate = true; }
	void setDefaultPos(const glm::vec2& newDefault) { m_defaultPos = newDefault; }

	//getters
	glm::vec2 getPosition() const { return m_position; }
	float getScale() const { return m_scale; }
	glm::mat4 getcamMatrix() const { return m_camMatrix; }
	glm::vec2 getDefaultPos() const { return m_defaultPos; }

private: 
	bool m_needsUpdate = true;
	float m_scale = 0.0f;
	glm::vec2 m_position;
	glm::mat4 m_camMatrix;
	glm::mat4 m_orthoMatrix;
	Window* m_window = nullptr;
	glm::vec2 m_size;
	glm::vec2 m_defaultPos;
};

}

#endif //XENRO_CAMERA_DEFINED