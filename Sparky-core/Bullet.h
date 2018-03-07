#pragma once

#include <glm/glm.hpp>
#include <Bengine/SpriteBatch.h>
#include <string>

class Bullet
{
public:
	int _damage = 100;						 // damage given by our bomb
	Bullet(glm::vec2 pos, GLuint textureID, int lifeTime, int playerID, float radius);
	~Bullet();

	float _radius;                          // the radius upto which our bomb explodes in every direction
	bool lifeFinished();                    // to check the remaining life of our bomb to explode
	int remainingLife;         

	void draw(Bengine::SpriteBatch& spriteBatch);
	bool update();								// Returns true  when we are out of life, to be destroyed
	glm::vec2 getPosition() { return _position + glm::vec2(_dim.x / 2, _dim.y / 2); }	//the dimension have been added to get the position of the centre of the bullet
	glm::vec2 getDim() { return _dim; }
	int getPlayerID() { return _playerID; }      // returns the id of player who planted the bomb
	int getDamage() { return _damage; }          
	// returns the bomb data when it is planted
	std::string getData() { return std::to_string(_playerID) + "|" +  std::to_string(_position.x) + " " + std::to_string(_position.y) + "|"; }

private:
	
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;       //_direction is a vector of unit length
	glm::vec2 _position;
	glm::vec2 _dim;

	GLuint _textureID;		// the texture of the bullet
	int _playerID;			// the person  who shoots the bomb
	int _bulletType;		// the type of the bomb, for now its just a single type


};

