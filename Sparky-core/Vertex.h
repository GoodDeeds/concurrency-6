#pragma once

#include <GL/glew.h>

struct Vertex {
	struct Position {
		float x;
		float y;
	}position;

	struct Color {                               //GLubyte same as unsigned char = 1 byte as 0< each color value <255, only 3 is needed but 4 to make size multiple of 4
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	}color;             
};