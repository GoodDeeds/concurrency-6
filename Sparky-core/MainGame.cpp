#include "MainGame.h"

#include <iostream>
#include <string>

void fatalError(std::string errorString) {
	std::cout << errorString << std::endl;
	std::cout << "Press any key to continue.." << std::endl;
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(1);
}
MainGame::MainGame()
{
	_window = nullptr;	
	_screenHeight = 600;
	_screenWidth = 800;
	_gameState = GameState::PLAY;
}


MainGame::~MainGame()
{
}

void MainGame::run() {

	initSystems();

	gameLoop();
}

void MainGame::initSystems() {

	SDL_Init(SDL_INIT_EVERYTHING);                // Initializing SDL

	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);    // last option i.e flag can be modified to whether we want full screen, minimized, etc.
	if (_window == nullptr) {
		fatalError("Could not create SDL window");	
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);     // Creating opengl context and giving it to our _window.
	if (glContext == nullptr) {
		fatalError("SDL_GL cotext could not be created ");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("could not initialize glew ");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);                 //keeps 2 window as we can draw on one while clearing other , prevents flickering
	
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);                        // Clears color to this color when glClear calls gl_color_buffer_bit		
}

void MainGame::gameLoop() {

	while (_gameState != GameState::EXIT) {
		processInput();

		drawGame();
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {               // checking for any sort of event
		switch (evnt.type) {
			case SDL_QUIT :
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
				break;
		}
	}
}

void MainGame::drawGame() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	   // clearing color buffer and depth buffer

	glEnableClientState(GL_COLOR_ARRAY);
	glBegin(GL_TRIANGLES);	
	glColor3f(1.0, 0.0f, 0.0f);
	glVertex2f(0, 0);
	glVertex2f(0, 250);
	glVertex2f(250, 250);
	glEnd();

	SDL_GL_SwapWindow(_window);
}

