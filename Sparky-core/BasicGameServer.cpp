#include "BasicGameServer.h"
#include <Bengine/Errors.h>
#include <Bengine/ResourceManager.h>

#include <iostream>
#include <string>

#include <thread>
#include <cmath>


BasicGameServer::BasicGameServer(int noOfPlayers, int currentIndex, const std::vector<Player>& players, socketServer* sockServer)
	:_screenWidth(800), _screenHeight(600), _time(0.0f), _gameState(GameState1::PLAY), _maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight);
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

void BasicGameServer::initSystems() {

	Bengine::init();

	_window.create("Server", _screenWidth, _screenHeight, 0);

	initShaders();

	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);


	//_leveldata = _levels[m_currentLevel]->getLevelData();
	for (int i = 0; i < _noOfPlayers; i++)
	{
		_chars.emplace_back(_players[i].name, _players[i].position, _players[i].playerIndex, _playerDim, 1 /*, _leveldata*/);
	}

	_mainPlayer = &(_chars[_currentIndex]);

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
		for (int i = 0; i < _bullets.size();) {
			if (_bullets[i].update() == true) {
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else {
				i++;
			}
		}

		drawGame();

		std::string temp = _mainPlayer->getData() + "0|1";
		socket->sendData(temp);

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
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (_inputManager.isKeyDown(SDLK_s)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (_inputManager.isKeyDown(SDLK_a)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyDown(SDLK_d)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
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

		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);    // normalise vector to unit length

		std::cout << "emitting in direction " << direction.x << " " << direction.y << " " << playerPosition.x << " " << playerPosition.x << std::endl;
		_bullets.emplace_back(playerPosition, direction, 1.00f, 1000);
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

	//for drawing the clients
	for (int i = 0; i < _noOfPlayers; i++)
	{
		_chars[i].draw(_spriteBatch);
	}

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	_window.swapBuffer();
}


