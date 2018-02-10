#include "MainGame.h"
#include "Errors.h"
#include "ImageLoader.h"

#include <iostream>
#include <string>


MainGame::MainGame() :_screenWidth(800), _screenHeight(600), _time(0.0f), _window(nullptr), _gameState(GameState::PLAY)
{
}


MainGame::~MainGame()
{
}

void MainGame::run() {

	initSystems();

	_sprite.init(-1.0f, -1.0f, 2.0f, 2.0f);               // Normalised coordinates
	
	_playerTexture = ImageLoader::loadPNG("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	
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

	initShaders();
}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop() {

	while (_gameState != GameState::EXIT) {
		processInput();

		_time += 0.01;

		drawGame();
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {               // checking for any sort of event
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;
		}
	}
}

void MainGame::drawGame() {
	glClearDepth(1.0);                                     // Setting base depth to 1.0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	   // clearing color buffer and depth buffer

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);                         // used for multitexturing
	glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);                       // 0 as we have used GL_TEXTURE0

	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	_sprite.draw();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window);
}

