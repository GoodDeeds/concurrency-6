#pragma once

#include <glm/glm.hpp>
#include <Bengine/SpriteBatch.h>

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();
	
	void draw(Bengine::SpriteBatch& spriteBatch);
	bool update();			// Returns true when we are out of life, to be destroyed
private:
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;       //_direction is a vector of unit length
	glm::vec2 _position;
};

