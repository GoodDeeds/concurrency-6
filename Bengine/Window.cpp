#include "Window.h"
#include "Errors.h"

namespace Bengine {

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);    // last option i.e flag can be modified to whether we want full screen, minimized, etc.
		if (_sdlWindow == nullptr) {
			fatalError("Could not create SDL window");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);     // Creating opengl context and giving it to our _window.
		if (glContext == nullptr) {
			fatalError("SDL_GL cotext could not be created ");
		}

		//Set up glew(optional but recommended)
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("could not initialize glew ");
		}

		//Checking openGl version
		std::printf("*** OpenGL version  =  %s ***", glGetString(GL_VERSION));

		//Setting background color to blue
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);                        // Clears color to this color when glClear calls gl_color_buffer_bit	

																	 //Set VSYNC
		SDL_GL_SetSwapInterval(0);     // 0 is off , 1 is on

		return 0;
	}

	void Window::swapBuffer() {
		SDL_GL_SwapWindow(_sdlWindow);
	}
}