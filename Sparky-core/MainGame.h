#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <vector>

#include "GLSLProgram.h"
#include "Sprite.h"
#include "GLTexture.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();                   // to run our game



private:
	void initSystems();           // to initialise opengl and sdl and our game window
	void initShaders();
	void processInput();
	void gameLoop();
	void drawGame();
	void calculateFPS();

	SDL_Window * _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	std::vector <Sprite*> _sprites;

	GLSLProgram _colorProgram;

	float _fps;
	float _maxFPS;
	float _frameTime;

	float _time;

};

