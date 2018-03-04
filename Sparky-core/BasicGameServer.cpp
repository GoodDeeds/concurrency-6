#include "BasicGameServer.h"
#include <Bengine/Errors.h>
#include <Bengine/ResourceManager.h>

#include <iostream>
#include <string>

#include <thread>
#include <cmath>
#include <fstream>


BasicGameServer::BasicGameServer(int noOfPlayers, int currentIndex, const std::vector<Player>& players, socketServer* sockServer)
	:_screenWidth(800), _screenHeight(600), _time(0.0f), _gameState(GameState1::PLAY), _maxFPS(60.0f),_currentLevel(0)
{
	
	socket = sockServer;

	_playerDim = glm::vec2(30.0f, 30.0f);
	_bulletDim = glm::vec2(5.0f, 5.0f);
	_noOfPlayers = noOfPlayers;
	_currentIndex = currentIndex;
	_players = players;
}


BasicGameServer::~BasicGameServer()
{
}

void BasicGameServer::run() {

	initSystems();

	//sending data one time
	std::string temp = _mainPlayer->getData() + "0|";
	socket->sendData(temp);

	// only returns when the game ends
	gameLoop();
}

/*void SimpleGameServer::loadImage(std::string path, GLuint& ID)
{
	ID = ResourceManager::getTexture(path).id;
}*/

void BasicGameServer::upDownControl()
{
	if (_inputManager.isKeyPressed(SDLK_UP))
		_mainPlayer->moveUP();

	if (_inputManager.isKeyPressed(SDLK_DOWN))
		_mainPlayer->moveDOWN();
}

void BasicGameServer::rightLeftControl()
{
	if (_inputManager.isKeyPressed(SDLK_LEFT))
		_mainPlayer->moveLEFT();

	if (_inputManager.isKeyPressed(SDLK_RIGHT))
		_mainPlayer->moveRIGHT();
}

void BasicGameServer::initSystems() {

	Bengine::init();

	_window.create("Server", _screenWidth, _screenHeight, 0);

	initShaders();


	/*
	std::vector<glm::vec2> t_brickPosition;

	std::vector<std::string> t_levelData;

	glm::vec2 t_dim = glm::vec2(25.0f, 25.0f);
	bool t_toshow = true;

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
				t_brickPosition.push_back(glm::vec2(x, y));
			}
		}
	}

	*/
	for (int i = 0; i < 39; i++) {
		_bricks.emplace_back(i);
	}

	initLevels(_currentLevel);
	_camera.init(_screenWidth, _screenHeight);

	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);


	_leveldata = _levels[_currentLevel]->getLevelData();
	for (int i = 0; i < _noOfPlayers; i++)
	{
		if (i == _currentIndex)
			std::cout << " server spawn position ---- " << _players[i].position.x << " " << _players[i].position.y << std::endl;
		else
			std::cout << " client spawn position ---- " << _players[i].position.x << " " << _players[i].position.y << std::endl;
			_chars.emplace_back(_players[i].name, _players[i].position, _players[i].playerIndex, _playerDim, 10 , 0 /*, _leveldata*/);
		//else
		//	_chars.emplace_back(_players[i].name, _players[i].position, _players[i].playerIndex, _playerDim, 1, 1 /*, _leveldata*/);
	}

	_mainPlayer = &(_chars[_currentIndex]);


	_heartTexID = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/heart.png").id;
	_wandTexID = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/wand.png").id;
	_redTexID = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/red.png").id;
	_blueTexID = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/blue.png").id;
	_grayTexID = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/gray.png").id;

}
void BasicGameServer::initLevels(int level) {
	//_levels.push_back(new Level("../Sparky-core/Levels/level" + std::to_string(level + 1) + ".txt", _screenWidth, _screenHeight));
	_levels.push_back(new Level("../Sparky-core/Levels/level1.txt", _screenWidth, _screenHeight));

	std::cout << "level is pushed back" << std::endl;
}

void BasicGameServer::receiver()
{
	//while (m_gameState != GameStateServer::EXIT)
	//{
	std::string in;
	socket->receiveData(in);
	//std::cout <<"in"<< in<<std::endl;
	while (in[0] == 'i')				//the server reads from local data and hence is not a blocking call. We introduce while loop to prevent game running with initial data indicated by 'i'
		socket->receiveData(in);
	mtx.lock();
	data = std::string(in);
	std::cout << data << std::endl;
	mtx.unlock();
	//}
}

void BasicGameServer::initShaders() {
	_colorProgram.compileShaders("../Sparky-core/Shaders/colorShading.vert", "../Sparky-core/Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}


void BasicGameServer::gameLoop() {

	while (_gameState != GameState1::EXIT) {

		_fpsLimiter.begin();

		receiver();

		_inputManager.update();

		processInput();
		
		_time += 0.1;

		_camera.setPosition(_mainPlayer->getPosition());
		_camera.update();

		//Update all bullets
		/*for (int i = 0; i < _bullets.size();) {
			if (_bullets[i].update() == true) {
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else {
				i++;
			}
		}*/

		updateChars();
		updateBullets();

		//updateBricks();

		drawGame();

		std::string temp = _mainPlayer->getData() + std::to_string(newBullCount) + "|" + newBulls;
		socket->sendData(temp);

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

void BasicGameServer::updateBricks()
{
	/*for (int i = 0; i < _bricks.size(); i++)
	{
		glm::vec2 diff = (_mainPlayer->getPosition() - _bricks[i].getPosition());
		if (abs(diff.x) <= 30.0f && abs(diff.y) <= 30.0f && _bricks[i].getVisibility())
		{
			_mainPlayer->setBrickToPop(i);
			std::cout << "Popping " << std::endl;
			break;
		}
		else
			_mainPlayer->setBrickToPop(-1);
	}*/
}

void BasicGameServer::updateChars()
{
	mtx.lock();
	std::string tempData = data;
	mtx.unlock();
	//std::cout << "temp data  ==  "<<tempData << std::endl;

	if (tempData == "")
	{
		//_mainPlayer->update();
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

			//bulletType
			i++;
			temp = "";
			while (tempData[i] != '|')
			{
				temp += tempData[i];
				i++;
			}
			int bType = std::stoi(temp);

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

			//x-direction
			i++;
			temp = "";
			while (tempData[i] != ' ')
			{
				temp += tempData[i];
				i++;
			}
			float xD = std::stof(temp);

			//y direction

			i++;
			temp = "";
			while (tempData[i] != '|')
			{
				temp += tempData[i];
				i++;
			}
			float yD = std::stof(temp);

			i++;
			temp = "";

			static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/jimmyJump_pack/PNG/Bubble_Big.png");

			if (pID != _currentIndex) {
				std::cout << " adding bullets " << _bullets.size() << std::endl;
		
				_bullets.emplace_back(glm::vec2(xP, yP), glm::vec2(xD, yD),/* _bulletTexID[bType]*/ texture.id, 0.001f, 500, pID, bType, 20);
			}
				
		}
		
		if (j != _currentIndex)
			_chars[j].setData(x, y /*, health, score*/);
			
	}
	//_mainPlayer->update();
}


void BasicGameServer::updateBullets()
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
					//here 20.0f is the tile width
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

				if (abs(abs(bulPos.x - playerPos.x) - (_playerDim.x / 2 + _bulletDim.x / 2) ) <= _bullets[i]._radius &&
					abs(abs(bulPos.y - playerPos.y) - (_playerDim.y / 2 + _bulletDim.y / 2)) <= _bullets[i]._radius)
				{
					std::cout << "Player damaged " << std::endl;

					if (_chars[j].damageTaken(_bullets[i].getDamage()))
					{
						std::cout << "Player dead " << std::endl;
						//if (_bullets[i].getPlayerID() == _currentIndex)
						//_mainPlayer->increaseScore();
					}
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					continue;
				}
			}

			if (_bullets[i].update(/*_leveldata*/))         
			{
				
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else
				i++;
		}
	}
}
/*

void BasicGameServer::updateBullets()
{
	for (unsigned int i = 0; i < _bullets.size();)
	{
		bool ifBulletDamaged = false;
		for (int j = 0; j < _noOfPlayers; j++)
		{
			glm::vec2 bulPos = _bullets[i].getPosition();
			glm::vec2 playerPos = _chars[j].getPosition();

			if (_bullets[i].remainingLife == 1)
			{
				std::cout << "Lifetime finished " << std::endl;

				if (abs(abs(bulPos.x - playerPos.x) - (_playerDim.x / 2 + _bulletDim.x / 2)) <= _bullets[i]._radius &&
					abs(abs(bulPos.y - playerPos.y) - (_playerDim.y / 2 + _bulletDim.y / 2)) <= _bullets[i]._radius)
				{
					ifBulletDamaged = true;

					std::cout << "Player damaged = " << j <<  std::endl;

					if (_chars[j].damageTaken(_bullets[i].getDamage()))
					{
						std::cout << "Player dead " << std::endl;
						//if (_bullets[i].getPlayerID() == _currentIndex)
						//_mainPlayer->increaseScore();
					}
					
				}
			}
		}
		if (ifBulletDamaged)
		{
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
			ifBulletDamaged = false;
			continue;
		}
		if (_bullets[i].update())
		{
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else
			i++;
	}
}
*/

void BasicGameServer::processInput() {
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {               // checking for any sort of event
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState1::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			std::cout << "x pos = " << evnt.motion.x << " y = " << evnt.motion.y << std::endl;
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

	if (_inputManager.isKeyDown(SDLK_w)) {
		_mainPlayer->moveUP();
	}
	if (_inputManager.isKeyDown(SDLK_s)) {
		_mainPlayer->moveDOWN();
	}
	if (_inputManager.isKeyDown(SDLK_a)) {
		_mainPlayer->moveLEFT();
	}
	if (_inputManager.isKeyDown(SDLK_d)) {
		_mainPlayer->moveRIGHT();
	}
	if (_inputManager.isKeyDown(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		glm::vec2 playerPosition = _mainPlayer->getPosition();
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);    // normalise vector to unit length

		std::cout << "emitting in direction server " << direction.x << " " << direction.y << " " << playerPosition.x << " " << playerPosition.x << std::endl;
		//_bullets.emplace_back(playerPosition, direction, 1.00f, 1000);
	
		static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/jimmyJump_pack/PNG/Bubble_Big.png");


		_bullets.emplace_back(playerPosition, direction, /* _bulletTexID[bType]*/ texture.id, 0.001f, 500, _currentIndex, 1, 20);

			newBulls += _bullets[_bullets.size() - 1].getData();
			newBullCount++;
	
	}
}

void BasicGameServer::drawGame() {
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

	/*glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	Bengine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;


	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);
	*/
	//_spriteBatch.draw(pos + glm::vec4(0, 60 , 0, 0), uv, texture.id, 0.0f, color);

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_spriteBatch);
	}

	for (int i = 0; i < _bricks.size(); i++) {
		if(_bricks[i].getVisibility())
			_bricks[i].draw(_spriteBatch);
	}

	//for drawing the clients
	for (int i = 0; i < _noOfPlayers; i++)
	{
		_chars[i].draw(_spriteBatch);
	}


	float health = _mainPlayer->getHealth();
	float mana = 200;// _mainPlayer->getMana();
	_heartPos = _camera.convertScreenToWorld(glm::vec2(40.0f, 40.0f));
	_spriteBatch.draw(glm::vec4(_heartPos.x, _heartPos.y, _heartDim.x, _heartDim.y), _uv, _heartTexID, 5, _color);
	_spriteBatch.draw(glm::vec4(_heartPos.x + 1.5*_heartDim.x, _heartPos.y, health / 4, _heartDim.y), _uv, _redTexID, 5, _color);
	_spriteBatch.draw(glm::vec4(_heartPos.x + 1.5*_heartDim.x + health / 4, _heartPos.y, 50.0f - health / 4, _heartDim.y), _uv, _grayTexID, 5, _color);
	_spriteBatch.draw(glm::vec4(_heartPos.x, _heartPos.y - 2 * _heartDim.y, _heartDim.x, _heartDim.y), _uv, _wandTexID, 5, _color);
	_spriteBatch.draw(glm::vec4(_heartPos.x + 1.5*_heartDim.x, _heartPos.y - 2 * _heartDim.y, mana / 2, _heartDim.y), _uv, _blueTexID, 5, _color);
	_spriteBatch.draw(glm::vec4(_heartPos.x + 1.5*_heartDim.x + mana / 2, _heartPos.y - 2 * _heartDim.y, 50.0f - mana / 2, _heartDim.y), _uv, _grayTexID, 5, _color);

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	_window.swapBuffer();
}


