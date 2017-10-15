#pragma once
#include <GL\glew.h>

namespace Xenro {

	struct Position {
		Position(float X, float Y)
			:x(X), y(Y)
		{}
		Position()
			:x(0), y(0)
		{}

		float x;
		float y;
	};

	//4 bytes for r g b a color.
	struct ColorRGBA {
		ColorRGBA()
			:r(255), g(255), b(255), a(255)
		{}

		ColorRGBA(GLubyte R, GLubyte G, GLubyte B, GLubyte A)
			:r(R), g(G), b(B), a(A)
		{}

		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct UV {
		UV(float U, float V)
			:u(U), v(V)
		{}
		UV()
			:u(0), v(0)
		{}

		float u;
		float v;
	};

	//the vertex definition.
	struct Vertex {

		Position position;
		ColorRGBA color;
		//UV texture coordinates.
		UV uv;

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}
	};
}