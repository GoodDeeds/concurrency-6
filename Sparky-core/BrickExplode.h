#pragma once

#include <glm/glm.hpp>
#include <Bengine/SpriteBatch.h>
#include <string>
#include <Bengine/GLTexture.h>
#include "Level.h"

class BrickExplode
{
public:
	BrickExplode();
	BrickExplode( int index , glm::vec2 brickPosition);
	~BrickExplode();

	void draw(Bengine::SpriteBatch& spriteBatch);


	glm::vec2 getPosition() { return _position /*+ glm::vec2(_dim.x / 2, _dim.y / 2)*/; }	//the dimension have been added to get the position of the centre of the bullet


	int getBrickIndex() const { return _index; }

	bool getVisibility();
	void setVisibility(bool setValue);

	
	
private:
	//std::vector<glm::vec2> brickPosition;

	std::vector<std::string> _levelData;

	int _index;
	glm::vec2 _position;
	glm::vec2 _dim;

	bool _toshow;

	

	// { glm::vec2(100.0f,30.0f),glm::vec2(150.0f,0.0f), glm::vec2(200.0f, 50.0f), glm::vec2(-150.0f,-50.0f), glm::vec2( 300.0f, -30.0f)};

};

