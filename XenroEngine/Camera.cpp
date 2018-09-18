/*************************************************************************/
/*                            Camera.cpp                                 */
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

#include "Camera.h"
#include "Window.h"
#include "GLSLProgram.h"
#include <algorithm>

namespace Xenro {

Camera::Camera()
	:m_camMatrix(1.0f), m_position(0.0f, 0.0f), m_needsUpdate(true),
	m_scale(1.0f), m_defaultPos(0.0f,0.0f), m_size(0,0)
{
	//Empty
}

Camera::Camera(Window* window)
	:m_camMatrix(1.0f), m_position(0.0f, 0.0f), m_needsUpdate(true), 
	 m_scale(1.0f), m_window(window), m_defaultPos(0.0f,0.0f)
{
	m_orthoMatrix = glm::ortho(0.0f, (float)m_window->getScreenWidth(), 0.0f, (float)m_window->getScreenHeight());
	m_size = glm::vec2(m_window->getScreenWidth(), m_window->getScreenHeight());
}


Camera::~Camera()
{
	//Empty
}

//Initialiaze our camera with screen size info.
void Camera::reset(Window* window) {

	m_window = window;
	m_size = glm::vec2(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_position = m_defaultPos;
	m_scale = 1.0f;
	m_camMatrix = glm::mat4(1.0f);
	m_orthoMatrix = glm::ortho(0.0f, (float)m_window->getScreenWidth(), 0.0f, (float)m_window->getScreenHeight());
	m_needsUpdate = true;
}

//Move the camera when an update is needed.
void Camera::update() {
	
	//check if SDL window was resized.
	if (m_size.x != m_window->getScreenWidth() || m_size.y != m_window->getScreenHeight()) {
		reset(m_window);
	}

	if (m_needsUpdate) {

		//Camera Translation
		glm::vec3 translate(-m_position.x + m_window->getScreenWidth() / 2.0f, -m_position.y + m_window->getScreenHeight() / 2.0f, 0.0f);
		m_camMatrix = glm::translate(m_orthoMatrix, translate);

		//Camera Scale
		glm::vec3 scale(m_scale, m_scale, 0.0f);
		m_camMatrix = glm::scale(glm::mat4(1.0f), scale) * m_camMatrix;

		//No longer needs to be updated.
		m_needsUpdate = false;
	}
}

glm::vec2 Camera::convertScreentoWorld(glm::vec2 screenCoords) {

	//invert y direction.
	screenCoords.y = m_window->getScreenHeight() - screenCoords.y;

	//Make zero the center.
	screenCoords -= glm::vec2(m_window->getScreenWidth() / 2.0f, m_window->getScreenHeight() / 2.0f);

	//Scale the coordinates.
	screenCoords /= m_scale;

	//Translate with the camera position.
	screenCoords += m_position;

	return screenCoords;
}

bool Camera::isInCam(const glm::vec2& position, const glm::vec2& dimensions) {

	glm::vec2 camScaleDim = glm::vec2(m_window->getScreenWidth(), m_window->getScreenHeight()) / m_scale;

	//divide dimensions by 2 since we are searching from center.
	const float MIN_DISTANCEX = dimensions.x / 2.0f + camScaleDim.x / 2.0f;
	const float MIN_DISTANCEY = dimensions.y / 2.0f + camScaleDim.y / 2.0f;

	glm::vec2 centerActorPos = position + dimensions / 2.0f;
	glm::vec2 centerCamPos = m_position;
	glm::vec2 dist = centerCamPos - centerActorPos;

	float xdepth = MIN_DISTANCEX - abs(dist.x);
	float ydepth = MIN_DISTANCEY - abs(dist.y);

	//check for collision.
	if (xdepth > 0.0f && ydepth > 0.0f) {
		return true;
	}
	return false;
}

void Camera::updateUniform(GLSLProgram* textureProgram, const std::string& uniform) {
	//Set the camera matrix.
	glm::mat4 cameraMatrix = getcamMatrix();
	GLint pLocation = textureProgram->getUniformLocation("P");

	//Pass pointer to openGL
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
}

}