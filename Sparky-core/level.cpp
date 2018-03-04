#include "Level.h"
#include <fstream>
#include <Bengine/Errors.h>
#include <iostream>
#include <Bengine/ResourceManager.h>
#include <glm/glm.hpp>



Level::Level(const std::string &fileName, int screenWidth, int screenHeight) {

	//grassTexid = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/grass.png").id;

	std::ifstream file;
	//fileName = "../Sparky-core/Levels/level1.txt";
	file.open(fileName);
	if (file.fail()) {
		Bengine::fatalError("ohMyGOD!!! The " + fileName + " level could not be loaded!");
	}

	/*std::string tmp = "";
	file >> tmp;
	_numHumans = std::stoi(tmp);
	*/

	//std::getline(file, tmp); //throw away first line;
	std::string tmp;
	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
	}

	const static glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::Color color;
	color.set(255, 0, 255, 255);

	_spriteBatch.init();
	_spriteBatch.begin();

	for (unsigned int y = 0; y < _levelData.size(); y++) {
		for (unsigned int x = 0; x < _levelData[y].size(); x++) {

			char tile = _levelData[y][x];
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			switch (tile) {
			case 'R':
				std::cout << "rendering R " << std::endl;
				_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("../Sparky-core/Textures/red_bricks.png").id,
					0.0f,
					color);
				break;
			case 'C':
				std::cout << "rendering C " << std::endl;
				_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("../Sparky-core/Textures/glass.png").id,
					0.0f,
					color);
				break;
			case 'B':
				std::cout << "rendering B " << std::endl;
				_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("../Sparky-core/Textures/light_bricks.png").id,
					0.0f,
					color);
				break;
			case '@':
				std::cout << "rendering @ " << std::endl;
				//_playerStartPos.x = x * TILE_WIDTH;
				//_playerStartPos.y = y * TILE_WIDTH;
				_levelData[x][y] = '.'; //Homogenize our collision cases
				break;
			case 'Z':
				std::cout << "rendering Z " << std::endl;
				//_zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				_levelData[x][y] = '.'; //Homogenize our collision cases
				break;
			default:
				break;
			}

		}
	}
	_spriteBatch.end();
	file.close();
}


Level::~Level()
{
}

void Level::draw() {
	_spriteBatch.renderBatch();
}
