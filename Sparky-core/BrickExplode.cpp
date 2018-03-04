#include "BrickExplode.h"
#include <iostream>
#include <Bengine/ResourceManager.h>
#include <fstream>
#include <Bengine/Errors.h>


BrickExplode::BrickExplode()
{
}

BrickExplode::BrickExplode(int index)
{
	_index = index;
	
		_dim = glm::vec2(25.0f, 25.0f);
		_toshow = true;

		std::ifstream file;
		std::string fileName = "../Sparky-core/Levels/level1.txt";
		file.open(fileName);
		if (file.fail()) {
			Bengine::fatalError("ohMyGOD!!! The " + fileName + " level could not be loaded!");
		}


		std::string tmp;
		while (std::getline(file, tmp)) {
			_levelData.push_back(tmp);
		}


		for (int y = 0; y < _levelData.size(); y++) {
			for (int x = 0; x < _levelData[y].size(); x++) {

				if (_levelData[y][x] == 'B')
				{
					std::cout << "adding x = " << x << " adding y = " << y << std::endl;
					brickPosition.push_back(glm::vec2(x, y));
				}
			}
		}
	
	
	_position = brickPosition[index];
}



BrickExplode::~BrickExplode()
{
}

bool BrickExplode::getVisibility()
{
	return (_toshow);
}
void BrickExplode::setVisibility(bool SetValue)
{
	_toshow = SetValue;
	std::cout << "setting false" << std::endl;
}


void BrickExplode::draw(Bengine::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/red_bricks.png");
	Bengine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	//glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);

	//spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);

	//the dimensions of bullet has been subtracted to centre the bullet
	spriteBatch.draw(glm::vec4((_position.x /*- _dim.x / 2*/ ) * 20, (_position.y /*- _dim.y / 2*/) * 20, _dim.x, _dim.y), uv, texture.id, 0.0f, color);
}
