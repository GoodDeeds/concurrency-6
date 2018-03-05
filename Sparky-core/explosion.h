#pragma once
#include <Bengine/SpriteBatch.h>
#include <Bengine/GLTexture.h>
#include <Bengine/ResourceManager.h>
#include <glm/glm.hpp>

class explosion
{
public:
	explosion(glm::vec2 pos);
	~explosion();

	void draw(Bengine::SpriteBatch& spriteBatch);

	bool updateTimer();

private:
	glm::vec2 _pos;
	glm::vec2 _dim;
	int timer = 500;





};

