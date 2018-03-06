#include "Bullet.h"

#include <Bengine/ResourceManager.h>



Bullet::Bullet(glm::vec2 pos, GLuint textureID , int lifeTime, int playerID, float radius)
{
	_lifeTime = 100;
	remainingLife = _lifeTime;
	_position = pos;
	_textureID = textureID;
	_playerID = playerID;

	_dim = glm::vec2(30.0f, 30.0f);

	_radius = radius;
}

Bullet::~Bullet()
{
}


void Bullet::draw(Bengine::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	Bengine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	spriteBatch.draw(glm::vec4(_position.x - _dim.x / 2, _position.y - _dim.y / 2, _dim.x, _dim.y), uv, _textureID, 0.0f, color);
}

bool Bullet::update() {
	_lifeTime--;

	remainingLife = _lifeTime;

	if (_lifeTime <= 0) {
		return true;
	}
	return false;
}

bool Bullet::lifeFinished() {
	if (_lifeTime <= 0) {
		return true;
	}
	else
		return false;
}