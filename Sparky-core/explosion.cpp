#include "explosion.h"



explosion::explosion(glm::vec2 pos)
{
	_pos = pos;
	_dim = glm::vec2(20.0f, 20.0f);
}

explosion::~explosion()
{
}

void explosion::draw(Bengine::SpriteBatch & spriteBatch)
{
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("../Sparky-core/Textures/AngryCloud.png");
	Bengine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	//glm::vec4 posAndSize = glm::vec4(_pos.x, _pos.y, 30, 30);

	//spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);

	//the dimensions of bullet has been subtracted to centre the bullet
	spriteBatch.draw(glm::vec4(_pos.x , _pos.y , _dim.x, _dim.y), uv, texture.id, 0.0f, color);
}

bool explosion::updateTimer()
{
	timer--;
	if (timer > 0)
		return true;
	else
		return false;
}
