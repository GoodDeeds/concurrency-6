#include <SDL/SDL.h>
#include <GL/glew.h>

#include "Bengine.h"

namespace Bengine {

	int init() {

		SDL_Init(SDL_INIT_EVERYTHING);                // Initializing SDL

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);                 //keeps 2 window as we can draw on one while clearing other , prevents flickering

		return 0;
	}
}