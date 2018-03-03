#pragma once

#include <glm/glm.hpp>
#include <Bengine/SpriteBatch.h>
#include <string>

class Bullet
{
public:
	int _damage = 100;
	Bullet(glm::vec2 pos, glm::vec2 dir, GLuint textureID, float speed, int lifeTime, int playerID, int bulletType);
	~Bullet();

	bool lifeFinished();
	
	void draw(Bengine::SpriteBatch& spriteBatch);
	bool update();			// Returns true when we are out of life, to be destroyed
	glm::vec2 getPosition() { return _position + glm::vec2(_dim.x / 2, _dim.y / 2); }	//the dimension have been added to get the position of the centre of the bullet
	glm::vec2 getDim() { return _dim; }
	int getPlayerID() { return _playerID; }
	int getDamage() { return _damage; }
	std::string getData() { return std::to_string(_playerID) + "|" + std::to_string(_bulletType) + "|" + std::to_string(_position.x) + " " + std::to_string(_position.y) + "|" + std::to_string(_direction.x) + " " + std::to_string(_direction.y) + "|"; }
private:
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;       //_direction is a vector of unit length
	glm::vec2 _position;
	glm::vec2 _dim;

	GLuint _textureID;//the texture of the bullet
	int _playerID;	//the person who shot the bullet
	int _bulletType;//the type of the bullet


};

