#include "Bullet.h"

#include <Bengine/ResourceManager.h>



Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, GLuint textureID, float speed, int lifeTime, int playerID, int bulletType)
{
	_lifeTime = lifeTime;
	_position = pos;
	_direction = dir;
	_speed = speed;

	_textureID = textureID;
	_playerID = playerID;
	_bulletType = bulletType;

	_dim = glm::vec2(5.0f, 5.0f);
}

Bullet::~Bullet()
{
}


void Bullet::draw(Bengine::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	//static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/jimmyJump_pack/PNG/Bullet.png");
	Bengine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	//glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);

	//spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);

	//the dimensions of bullet has been subtracted to centre the bullet
	spriteBatch.draw(glm::vec4(_position.x - _dim.x / 2, _position.y - _dim.y / 2, _dim.x, _dim.y), uv, _textureID, 0.0f, color);
}

bool Bullet::update() {
	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime == 0) {
		return true;
	}
	return false;
}