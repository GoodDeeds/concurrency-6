#include "BrickExplode.h"
#include <iostream>
#include <Bengine/ResourceManager.h>
#include <fstream>
#include <Bengine/Errors.h>


BrickExplode::BrickExplode()
{
}


BrickExplode::BrickExplode(int index, glm::vec2 brickPosition)
{
	_index = index;
	
	_dim = glm::vec2(20.0f, 20.0f);
	_toshow = true;

	_position = brickPosition;
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
}


void BrickExplode::draw(Bengine::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/light_bricks.png");
	Bengine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	spriteBatch.draw(glm::vec4((_position.x /*- _dim.x / 2*/ ) * 20, (_position.y /*- _dim.y / 2*/) * 20, _dim.x, _dim.y), uv, texture.id, 0.0f, color);
}
