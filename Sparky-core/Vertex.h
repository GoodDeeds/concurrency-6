#pragma once

#include <GL/glew.h>

struct Position {
	float x;
	float y;
};

struct Color {                               //GLubyte same as unsigned char = 1 byte as 0< each color value <255, only 3 is needed but 4 to make size multiple of 4
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct UV {
	float u;
	float v;
};

struct Vertex {
	Position position;

	Color color;  
	
	UV uv;             //uv texture coordinates

	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}

	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	void setUV(float u, float v) {
		uv.u = u;
		uv.v = v;
	}
};