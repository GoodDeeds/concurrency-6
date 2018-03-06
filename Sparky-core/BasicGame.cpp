#include "BasicGame.h"
#include <Bengine/Errors.h>
#include <Bengine/ResourceManager.h>

#include <iostream>
#include <string>

#include <thread>
#include <cmath>
#include <fstream>



BasicGame::BasicGame(int noOfPlayers, int currentIndex, const std::vector<Player>& players, socketClient* sockClient)
	:_screenWidth(800), _screenHeight(600), _time(0.0f), _gameState(GameState2::PLAY), _maxFPS(60.0f),_currentLevel(0)

{
	socket = sockClient;

	_playerDim = glm::vec2(45.0f, 45.0f);
	_bulletDim = glm::vec2(30.0f, 30.0f);
	_noOfPlayers = noOfPlayers;
	_currentIndex = currentIndex;
	_players = players;
}


BasicGame::~BasicGame()
{
}

void BasicGame::run() {

	initSystems();

	char d[1000];
	strcpy_s(d, _mainPlayer->getData().c_str());
	newBulls = "0|" + newBulls;
	strcat_s(d, newBulls.c_str());

	socket->sendBytes(d);

	newBulls = "";
	newBullCount = 0;

	// only returns when the game ends
	gameLoop();
}


void BasicGame::initSystems() {

	Bengine::init();

	_window.create("Client", _screenWidth, _screenHeight, 0);

	initShaders();


	std::ifstream t_file;
	std::string t_fileName = "../Sparky-core/Levels/level1.txt";
	t_file.open(t_fileName);
	if (t_file.fail()) {
		Bengine::fatalError("ohMyGOD!!! The " + t_fileName + " level could not be loaded!");
	}

	std::string tmp;
	while (std::getline(t_file, tmp)) {
		t_levelData.push_back(tmp);
	}

	for (int y = 0; y < t_levelData.size(); y++) {
		for (int x = 0; x < t_levelData[y].size(); x++) {

			if (t_levelData[y][x] == 'B')
			{
				std::cout << "adding x = " << x << " adding y = " << y << std::endl;
				t_brickPosition.push_back(glm::vec2(y, x));
			}

			if (t_levelData[y][x] == 'C')
			{
					std::cout << "adding C x = " << x << " adding y = " << y << std::endl;
					_brickFixed.push_back(glm::vec2(y, x));

			}
		}
	}


	for (int i = 0; i < t_brickPosition.size() ; i++) {
		_bricks.emplace_back(i , t_brickPosition[i]);
	}

	
	initLevels(_currentLevel);
	
	_camera.init(_screenWidth, _screenHeight);
	_camera.setScale(2);

	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
	_leveldata = _levels[_currentLevel]->getLevelData();
	for (int i = 0; i < _noOfPlayers; i++)
	{
		_chars.emplace_back(_players[i].name, _players[i].position, _players[i].playerIndex, _playerDim, 3, 0, _leveldata);
	}

	_mainPlayer = &(_chars[_currentIndex]);

	_heartTexID = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/heart.png").id;
	_redTexID = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/red.png").id;
	_grayTexID = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/gray.png").id;
}

void BasicGame::receiver()
{
	
	char in[1000];
	socket->receiveBytes(in);
	
	mtx.lock();
	data = std::string(in);
	mtx.unlock();
	
}

void BasicGame::initShaders() {
	_colorProgram.compileShaders("../Sparky-core/Shaders/colorShading.vert", "../Sparky-core/Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}
void BasicGame::initLevels(int level) {
	_levels.push_back(new Level("../Sparky-core/Levels/level" + std::to_string(level + 1) + ".txt", _screenWidth, _screenHeight));
	std::cout << "level is pushed back" << std::endl;
}
void BasicGame::gameLoop() {

	while (_gameState != GameState2::EXIT) {

		_fpsLimiter.begin();

		receiver();

		_inputManager.update();


		if (_mainPlayer->getHealth() <= 0)
		{
			std::cout << "You killed " << _mainPlayer->getScore() << " players " << std::endl;
			SDL_Quit();
		}
			

		processInput();

		_time += 0.1;

		_camera.setPosition(_mainPlayer->getPosition());
		_camera.update();

		updateChars();
		updateBullets();

		drawGame();

		char d[1000];
		strcpy_s(d, _mainPlayer->getData().c_str());
		newBulls = std::to_string(newBullCount) + "|" + newBulls;
		strcat_s(d, newBulls.c_str());


		socket->sendBytes(d);
		newBulls = "";
		newBullCount = 0;

		_fps = _fpsLimiter.end();

		//print once in 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10000) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}


	}
}

void BasicGame::updatePlayerLife()
{
	for (int i = 0; i < _chars.size(); i++)
	{
		if (_chars[i].getHealth() > 0.0f)
			_chars[i].setLife(true);
		else
			_chars[i].setLife(false);
	}
}

void BasicGame::updateChars()
{
	mtx.lock();
	std::string tempData = data;
	mtx.unlock();

	if (tempData == "")
	{
		return;
	}
	int i = 0;

	for (int j = 0; j < _noOfPlayers; j++)
	{
		std::string temp = "";
		//x-coordinate
		while (tempData[i] != ' ')
		{
			temp += tempData[i];
			i++;
		}
		float x = std::stof(temp);

		//y-coordinate
		i++;
		temp = "";
		while (tempData[i] != '|')
		{
			temp += tempData[i];
			i++;
		}
		float y = std::stof(temp);

		//health
		i++;
		temp = "";
		while (tempData[i] != '|')
		{
			temp += tempData[i];
			i++;
		}
		float health = std::stof(temp);

		//score
		i++;
		temp = "";
		while (tempData[i] != '|')
		{
			temp += tempData[i];
			i++;
		}

		int brickToPop = std::stoi(temp);

		if (_bricks.size() > 0 && brickToPop != -1 && brickToPop < _bricks.size())
		{
			std::cout << "setting" << std::endl;

			_bricks[brickToPop].setVisibility(false);

		}
		

		//no. Of Bullets
		temp = "";
		i++;
		while (tempData[i] != '|')
		{
			temp += tempData[i];
			i++;
		}
		int numBull = std::stoi(temp);

		//bullet DATA
		temp = "";
		i++;
		for (int z = 0; z < numBull; z++)
		{
			//id of the person shooting
			while (tempData[i] != '|')
			{
				temp += tempData[i];
				i++;
			}
			int pID = std::stoi(temp);

			//data of bullets

			//x position
			i++;
			temp = "";
			while (tempData[i] != ' ')
			{
				temp += tempData[i];
				i++;
			}
			float xP = std::stof(temp);

			//y position
			i++;
			temp = "";
			while (tempData[i] != '|')
			{
				temp += tempData[i];
				i++;
			}
			float yP = std::stof(temp);

			i++;
			temp = "";

			static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/bomb.png");

			if (pID != _currentIndex)
			{
				_bullets.emplace_back(glm::vec2(xP, yP), texture.id, 500, pID , 40);

			}
		}
		if (j != _currentIndex)
			_chars[j].setData(x, y, health);
	}
	
}

void BasicGame::updateExplosions()
{
	for (int i = 0; i < _explosions.size();)
	{
		if (_explosions[i].updateTimer()) { i++; }
		else
		{
			_explosions[i] = _explosions.back();
			_explosions.pop_back();
		}

	}
}

void BasicGame::processInput() {
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {               // checking for any sort of event
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState2::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}

	if (_inputManager.isKeyDown(SDLK_w)|| _inputManager.isKeyDown(SDLK_UP)) {
		_mainPlayer->moveUP(_bricks, _brickFixed);
	}
	if (_inputManager.isKeyDown(SDLK_s)|| _inputManager.isKeyDown(SDLK_DOWN)) {
		_mainPlayer->moveDOWN(_bricks, _brickFixed);
	}
	if (_inputManager.isKeyDown(SDLK_a)|| _inputManager.isKeyDown(SDLK_LEFT)) {
		_mainPlayer->moveLEFT(_bricks, _brickFixed);
	}
	if (_inputManager.isKeyDown(SDLK_d)|| _inputManager.isKeyDown(SDLK_RIGHT)) {
		_mainPlayer->moveRIGHT(_bricks, _brickFixed);
	}
	if (_inputManager.isKeyDown(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)|| _inputManager.isKeyDown(SDLK_SPACE)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		glm::vec2 playerPosition = _mainPlayer->getPosition();
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);    // normalise vector to unit length

		std::cout << "emitting in direction client " << direction.x << " " << direction.y << " " << playerPosition.x << " " << playerPosition.x << std::endl;
		
		static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/bomb.png");

		_bullets.emplace_back(playerPosition, texture.id, 500, _currentIndex, 40);

		newBulls += _bullets[_bullets.size() - 1].getData();
		newBullCount++;
	}
}


void BasicGame::updateBullets()
{
	for (int j = 0; j < _noOfPlayers; j++)
	{
		for (unsigned int i = 0; i < _bullets.size();)
		{
			glm::vec2 bulPos = _bullets[i].getPosition();
			glm::vec2 playerPos = _chars[j].getPosition();
			if (_bullets[i].getPlayerID() == j)
			{
				i++;
				continue;
			}

			std::cout << " remain life " << _bullets[i].remainingLife << std::endl;

			if (_bullets[i].remainingLife == 1)
			{
				std::cout << "Lifetime finished " << std::endl;
				
				for (int z = 0; z < _bricks.size(); z++)
				{
					float diffX = abs(bulPos.x - _bricks[z].getPosition().x * 20.0f);
					float diffY = abs(bulPos.y - _bricks[z].getPosition().y * 20.0f);

					if (diffX <= 30.0f && diffY <= 30.0f && _bricks[z].getVisibility())
					{
						_mainPlayer->setBrickToPop(z);
						std::cout << "Popping " << std::endl;
						break;
					}
					else
						_mainPlayer->setBrickToPop(-1);
				}
				

				if (abs(abs(bulPos.x - playerPos.x) - (_playerDim.x / 2 + _bulletDim.x / 2)) <= _bullets[i]._radius &&
					abs(abs(bulPos.y - playerPos.y) - (_playerDim.y / 2 + _bulletDim.y / 2)) <= _bullets[i]._radius)
				{
					std::cout << "Player damaged reducing health " << std::endl;

					if (_chars[j].damageTaken(_bullets[i].getDamage()))
					{
						_chars[j].setLife(false);
						std::cout << "Player dead " << std::endl;

						if (_bullets[i].getPlayerID() == _currentIndex)
							_mainPlayer->increaseScore();
					}
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					_explosions.emplace_back(glm::vec2(bulPos.x, bulPos.y));
					continue;
				}
			}


			if (_bullets[i].update(/*_leveldata*/))
			{
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				_explosions.emplace_back(glm::vec2(bulPos.x, bulPos.y));
			}
			else
				i++;
		}
	}
}


void BasicGame::drawGame() {
	glClearDepth(1.0);                                     // Setting base depth to 1.0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	   // clearing color buffer and depth buffer

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);                         // used for multitexturing
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);                       // 0 as we have used GL_TEXTURE0

														   /*GLint timeLocation = _colorProgram.getUniformLocation("time");
														   glUniform1f(timeLocation, _time);*/

														   //Set the camera matrix
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	//loading the matrix to the GPU
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	_levels[_currentLevel]->draw();
	_spriteBatch.begin();

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_spriteBatch);
	}

	/*for (int i = 0; i < _explosions.size(); i++) {
		_explosions[i].draw(_spriteBatch);
	}*/

	for (int i = 0; i < _noOfPlayers; i++)
	{
		if(_chars[i].isAlive())
			_chars[i].draw(_spriteBatch);
	}

	for (int i = 0; i < _bricks.size(); i++) {
		if (_bricks[i].getVisibility())
			_bricks[i].draw(_spriteBatch);
	}


	float health = _mainPlayer->getHealth();

	_heartPos = _camera.convertScreenToWorld(glm::vec2(40.0f, 40.0f));
	_spriteBatch.draw(glm::vec4(_heartPos.x, _heartPos.y, _heartDim.x, _heartDim.y), _uv, _heartTexID, 5, _color);
	_spriteBatch.draw(glm::vec4(_heartPos.x + 1.5*_heartDim.x, _heartPos.y, health / 4, _heartDim.y), _uv, _redTexID, 5, _color);
	_spriteBatch.draw(glm::vec4(_heartPos.x + 1.5*_heartDim.x + health / 4, _heartPos.y, 50.0f - health / 4, _heartDim.y), _uv, _grayTexID, 5, _color);

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	_window.swapBuffer();
}
