#ifndef XENRO_CAMERA_DEFINED
#define XENRO_CAMERA_DEFINED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Xenro {
	class Window;

	class Camera
	{
	public:
		Camera();
		Camera(Window* window);
		~Camera();

		void init(Window* window);

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
		bool m_needsUpdate = true;
		float m_scale = 0.0f;
		glm::vec2 m_position;
		glm::mat4 m_camMatrix;
		glm::mat4 m_orthoMatrix;
		Window* m_window = nullptr;

	};
}

#endif //XENRO_CAMERA_DEFINED