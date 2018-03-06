#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <vector>
#include"level.h"
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
#include "BrickExplode.h"
#include "explosion.h"
#include <mutex>

enum class GameState2 { PLAY, EXIT };

class BasicGame:public BrickExplode
{
public:
	BasicGame(int noOfPlayers, int currentIndex, const std::vector<Player>& players, socketClient* sockClient);
	~BasicGame();

	void run();                   // to run our game



private:
	void initSystems();           // to initialise opengl and sdl and our game window
	void initShaders();
	void initLevels(int level);
	void processInput();
	void gameLoop();
	void drawGame();
	void upDownControl();
	void rightLeftControl();

	void updateChars();
	void updateExplosions();
	void updateBullets();

	void receiver();

	Bengine::Window  _window;
	int _screenWidth;
	int _screenHeight;
	int _currentLevel;
	GameState2 _gameState;


	Bengine::GLSLProgram _colorProgram;
	Bengine::Camera2D _camera;

	Bengine::SpriteBatch _spriteBatch;

	Bengine::InputManager _inputManager;
	Bengine::FpsLimiter _fpsLimiter;
	std::vector<Level*> _levels;
	glm::vec2 _heartPos;
	glm::vec2 _heartDim = glm::vec2(10, 10);
	std::vector<std::string> _leveldata;

	std::vector<BrickExplode> _bricks;
	std::vector<explosion> _explosions;

	void updateBricks();


	GLuint _heartTexID, _redTexID, _grayTexID, _blueTexID, _wandTexID;

	glm::vec4 _uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	Bengine::Color _color = { 255,255,255,255 };

	std::vector<Bullet> _bullets;

	std::vector<glm::vec2> t_brickPosition;

	std::vector<glm::vec2> _brickFixed;

	std::vector<std::string> t_levelData;


	float _maxFPS;
	float _fps;
	float _time;

	glm::vec2 _playerDim, _bulletDim;
	std::vector<Character> _chars;
	Character* _mainPlayer;

	int _noOfPlayers, _currentIndex;
	std::string newBulls = "";
	int newBullCount = 0;
	std::vector<Player> _players;

	socketClient* socket;
	std::string data;
	int m_time;
	std::mutex mtx, timeMtx;
};
