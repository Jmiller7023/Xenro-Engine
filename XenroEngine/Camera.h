#ifndef XENRO_CAMERA_DEFINED
#define XENRO_CAMERA_DEFINED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Xenro {
	class Camera
	{
	public:
		Camera();
		Camera(int screenWidth, int screenHeight);
		~Camera();

		void init(int screenWidth, int screenHeight);

		void update();

		glm::vec2 convertScreentoWorld(glm::vec2 screenCoords);

		bool isInCam(const glm::vec2& position,const glm::vec2& dimensions);

		//setters
		void setPosition(const glm::vec2& newPosition) {
			m_position = newPosition; m_needsUpdate = true; }
		void setScale(float newScale) { m_scale = newScale; m_needsUpdate = true;
		}

		//getters
		glm::vec2 getPosition() const { return m_position; }
		float getScale() const { return m_scale; }
		glm::mat4 getcamMatrix() const { return m_camMatrix; }

	private: 
		bool m_needsUpdate;
		float m_scale;
		glm::vec2 m_position;
		glm::mat4 m_camMatrix;
		glm::mat4 m_orthoMatrix;
		int m_screenWidth, m_screenHeight;

	};
}

#endif //XENRO_CAMERA_DEFINED