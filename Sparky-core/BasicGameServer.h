#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <vector>

#include <Bengine/Bengine.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/Sprite.h>
#include <Bengine/GLTexture.h>
#include <Bengine/Window.h>
#include <Bengine/InputManager.h>
#include <Bengine/Timing.h>

#include <Bengine/SpriteBatch.h> 

#include <Bengine/Camera2D.h>

#include "Bullet.h"
#include "player.h"
#include "Sockets.h"
#include "Character.h"
#include <mutex>

enum class GameState1 { PLAY, EXIT };

class BasicGameServer
{
public:
	BasicGameServer(int noOfPlayers, int currentIndex, const std::vector<Player>& players, socketServer* sockServer);
	~BasicGameServer();

	void run();                   // to run our game



private:
	void loadImage(std::string path, GLuint& ID);

	void initSystems();           // to initialise opengl and sdl and our game window
	void initShaders();
	void processInput();
	void gameLoop();
	void drawGame();

	void receiver();

	void initialiseLevel(int currentLevel);
	void updateChars();

	//std::vector<Level*> _levels;

	int _currentLevel;
	std::vector<std::string> _leveldata;

	Bengine::Window  _window;
	int _screenWidth;
	int _screenHeight;
	GameState1 _gameState;


	Bengine::GLSLProgram _colorProgram;
	Bengine::Camera2D _camera;

	Bengine::SpriteBatch _spriteBatch;

	Bengine::InputManager _inputManager;
	Bengine::FpsLimiter _fpsLimiter;

	std::vector<Bullet> _bullets;

	glm::vec2 _playerDim, _bulletDim;
	std::vector<Character> _chars;
	Character* _mainPlayer;

	float _maxFPS;
	float _fps;
	float _time;

	int _noOfPlayers, _currentIndex;
	std::vector<Player> _players;

	socketServer* socket;
	std::string data;
	int m_time;
	std::mutex mtx, timeMtx;
};

