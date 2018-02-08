#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();                   // to run our game

	

private:
	void initSystems();           // to initialise opengl and sdl and our game window
	void processInput();
	void gameLoop();
	void drawGame();

	SDL_Window * _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

};

