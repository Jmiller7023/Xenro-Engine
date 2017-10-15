#include "Camera.h"
#include <algorithm>
namespace Xenro{

Camera::Camera()
	:m_camMatrix(1.0f) , m_position(0.0f, 0.0f), m_orthoMatrix(1.0f),
	m_needsUpdate(true), m_scale(1.0f), m_screenWidth(500), m_screenHeight(500)
{
	
}

Camera::Camera(int screenWidth, int screenHeight) 
	:m_camMatrix(1.0f), m_position(0.0f, 0.0f), m_needsUpdate(true), 
	 m_scale(1.0f), m_screenWidth(screenWidth), m_screenHeight(screenHeight)
{
	m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight);
}


Camera::~Camera()
{
}

//Initialiaze our camera with screen size info.
void Camera::init(int screenWidth, int screenHeight) {

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight);
}

//Move the camera when an update is needed.
void Camera::update() {
	if (m_needsUpdate) {
		//Camera Translation
		glm::vec3 translate(-m_position.x + m_screenWidth / 2, -m_position.y + m_screenHeight / 2, 0.0f);
		m_camMatrix = glm::translate(m_orthoMatrix, translate);

		//Camera Scale
		glm::vec3 scale(m_scale, m_scale, 0.0f);
		m_camMatrix = glm::scale(glm::mat4(1.0f), scale) * m_camMatrix;

		//No longer needs o be updated.
		m_needsUpdate = false;
	}
}

glm::vec2 Camera::convertScreentoWorld(glm::vec2 screenCoords) {
	//invert y direction.
	screenCoords.y = m_screenHeight - screenCoords.y;

	//Make zero the center.
	screenCoords -= glm::vec2(m_screenWidth / 2, m_screenHeight / 2);

	//Scale the coordinates.
	screenCoords /= m_scale;

	//Translate with the camera position.
	screenCoords += m_position;

	return screenCoords;
}

bool Camera::isInCam(const glm::vec2& position, const glm::vec2& dimensions) {

	glm::vec2 camScaleDim = glm::vec2(m_screenWidth, m_screenHeight) / m_scale;

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

}