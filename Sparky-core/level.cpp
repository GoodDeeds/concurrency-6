#include "Level.h"
#include <fstream>
#include <Bengine/Errors.h>
#include <iostream>
#include <Bengine/ResourceManager.h>
#include <glm/glm.hpp>


Level::Level()
{

}
Level::Level(const std::string &fileName, int screenWidth, int screenHeight) {

	//grassTexid = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/grass.png").id;

	std::ifstream file;
	//fileName = "../Sparky-core/Levels/level1.txt";
	file.open(fileName);
	if (file.fail()) {
		Bengine::fatalError("ohMyGOD!!! The " + fileName + " level could not be loaded!");
	}

	std::string tmp;
	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
	}

	const static glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::Color color;
	color.set(255, 0, 255, 255);

	_spriteBatch.init();
	_spriteBatch.begin();
	int count = 0;
	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData[y].size(); x++) {

			char tile = _levelData[y][x];
			glm::vec4 destRect(y * TILE_WIDTH, x * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			switch (tile) {
			case 'R':
				_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("../Sparky-core/Textures/bricksx64.png").id,
					0.0f,
					color);
				break;
			case 'C':
				_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("../Sparky-core/Textures/red_bricks.png").id,
					0.0f,
					color);
				break;
			case 'B':
				count++;
				
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
