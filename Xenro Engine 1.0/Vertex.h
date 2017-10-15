#pragma once
#include <GL\glew.h>


struct Position { 
	float x;
	float y;
}; 

struct Color {
	//4 bytes for r g b a color. (red green blue alpha (level of transparency))
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct UV {
	float u;
	float v;
};

//the vertex definition.
struct Vertex {

	Position position;
	Color color;
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