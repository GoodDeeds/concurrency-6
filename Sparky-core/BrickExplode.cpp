#include "BrickExplode.h"
#include <iostream>
#include <Bengine/ResourceManager.h>

BrickExplode::BrickExplode(int index)
{
	_index = index;
	_position = brickPosition[index];
	_dim = glm::vec2(25.0f, 25.0f);
	_toshow = true;
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
	spriteBatch.draw(glm::vec4(_position.x - _dim.x / 2, _position.y - _dim.y / 2, _dim.x, _dim.y), uv, texture.id, 0.0f, color);
}
