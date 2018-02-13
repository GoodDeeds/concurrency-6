#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <vector>

#include <Bengine/Bengine.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/Sprite.h>
#include <Bengine/GLTexture.h>
#include <Bengine/Window.h>

#include <Bengine/SpriteBatch.h> 

#include <Bengine/Camera2D.h>

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

	Bengine::Window  _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;


	Bengine::GLSLProgram _colorProgram;
	Bengine::Camera2D _camera;

	Bengine::SpriteBatch _spriteBatch;

	float _fps;
	float _maxFPS;
	float _frameTime;

	float _time;

};

