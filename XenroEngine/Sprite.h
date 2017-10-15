#pragma once
#include <GL\glew.h>
#include "GLTexture.h"

#include <string>

namespace Xenro {
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		//initialize our sprites.
		void init(float x, float y, float height, float width, std::string texturePath);

		//draw our sprites to the screen.
		void draw();

	private:
		float m_x;
		float m_y;
		float m_height;
		float m_width;
		GLuint m_vboID;
		GLTexture m_texture;
	};
}
