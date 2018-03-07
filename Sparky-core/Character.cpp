#include "Character.h"
#include <Bengine/ResourceManager.h>
#include <iostream>
#include <Bengine/GLTexture.h>
//#include"Level.h"
#include <cmath>
#include <iostream>



Character::Character(std::string name, glm::vec2 pos, int person, glm::vec2 dim, int speed, int choice , const std::vector<std::string>& levelData)
{
	m_health = 200.0f;
	m_name = name;
	m_position = pos;
	m_person = person;
	m_dim = dim;
	m_texId[0] = Bengine::ResourceManager::getTexture(m_filePaths[m_person]).id;
	m_speed = speed;
	m_levelData = levelData;
	m_choice = choice;
	m_score = -1;
	alive = true;
	score = 0;
}


void Character::init()
{
}

Character::~Character()
{
}

void Character::increaseScore()
{
	score++;
}

void Character::PlayerDead()
{
	alive = false;
}

void Character::setData(float x, float y , float health /*, int score*/)
{
	m_position.x = x;
	m_position.y = y;
	m_health = health;
}


void Character::setBrickToPop(int index) {
	m_score = index;
}

std::string Character::getData()
{
	std::string result = std::to_string(m_position.x) + " " + std::to_string(m_position.y) + "|" + std::to_string(m_health) + "|"  +std::to_string(m_score) + "|";
	return result;
}

void Character::draw(Bengine::SpriteBatch& spriteBatch)
{
	spriteBatch.draw(glm::vec4(m_position.x, m_position.y, m_dim.x, m_dim.y), m_uv, m_texId[0], 0.0f, m_color);
}



bool Character::damageTaken(int damage)
{
	m_health -= damage;
	if (m_health <= 0)
	{
		return true;
	}
	return false;
}


void Character::moveUP(std::vector<BrickExplode> &_bricks, std::vector<glm::vec2> _brickFixedPos)
{
	if (m_position.y > 930.0f)
		return;

	for (int i = 0; i < _brickFixedPos.size(); i++)
	{
		// 20.0f is tile width , 10.0f is half of brick dim
		if (abs(m_position.x + m_dim.x / 2 - _brickFixedPos[i].x*20.0f - 10.0f) < MIN_WALL_DISTANCE && abs(m_position.y + m_dim.y / 2 - _brickFixedPos[i].y * 20.0f - 10.0f) < MIN_WALL_DISTANCE && (m_position.y + m_dim.y / 2 - _brickFixedPos[i].y * 20.0f - 10.0f) < 0)
		{
			return;
		}
	}

	for (int i = 0; i < _bricks.size(); i++)
	{
		// 20.0f is tile width , 10.0f is half of brick dim
		if (_bricks[i].getVisibility() && abs(m_position.x + m_dim.x / 2 - _bricks[i].getPosition().x * 20.0f - 10.0f) < MIN_WALL_DISTANCE && abs(m_position.y + m_dim.y / 2 - _bricks[i].getPosition().y * 20.0f - 10.0f) < MIN_WALL_DISTANCE && (m_position.y + m_dim.y / 2 - _bricks[i].getPosition().y * 20.0f - 10.0f) < 0)
		{
			return;
		}
	}
	m_position += glm::vec2(0.0f, m_speed);
	return;
}
void Character::moveDOWN(std::vector<BrickExplode> &_bricks, std::vector<glm::vec2> _brickFixedPos)
{
	if (m_position.y < 23.0f)                   // down border screen coordinates
		return;

	for (int i = 0; i < _brickFixedPos.size(); i++)
	{
		if (abs(m_position.x + m_dim.x / 2 - _brickFixedPos[i].x * 20.0f - 10.0f) < MIN_WALL_DISTANCE && abs(m_position.y + m_dim.y / 2 - _brickFixedPos[i].y* 20.0f - 10.0f) < MIN_WALL_DISTANCE && (m_position.y + m_dim.y / 2 - _brickFixedPos[i].y * 20.0f - 10.0f) > 0)
		{
			return;
		}
	}


	for (int i = 0; i < _bricks.size(); i++)
	{
		if (_bricks[i].getVisibility() && abs(m_position.x + m_dim.x / 2 - _bricks[i].getPosition().x * 20.0f - 10.0f) < MIN_WALL_DISTANCE && abs(m_position.y + m_dim.y / 2 - _bricks[i].getPosition().y * 20.0f - 10.0f) < MIN_WALL_DISTANCE && (m_position.y + m_dim.y / 2 - _bricks[i].getPosition().y * 20.0f - 10.0f) > 0)
		{
			return;
		}
	}
	m_position += glm::vec2(0.0f, -m_speed);
	return;
}
void Character::moveLEFT(std::vector<BrickExplode> &_bricks, std::vector<glm::vec2> _brickFixedPos)
{
	if (m_position.x < 23.0f)
		return;

	for (int i = 0; i < _brickFixedPos.size(); i++)
	{
		if (abs(m_position.x + m_dim.x / 2 - _brickFixedPos[i].x * 20.0f - 10.0f) < MIN_WALL_DISTANCE && abs(m_position.y + m_dim.y / 2 - _brickFixedPos[i].y * 20.0f - 10.0f) < MIN_WALL_DISTANCE && (m_position.x + m_dim.x / 2 - _brickFixedPos[i].x * 20.0f - 10.0f) > 0)
		{
			return;
		}
	}

	for (int i = 0; i < _bricks.size(); i++)
	{
		if (_bricks[i].getVisibility() && abs(m_position.x + m_dim.x / 2 - _bricks[i].getPosition().x * 20.0f - 10.0f) < MIN_WALL_DISTANCE && abs(m_position.y + m_dim.y / 2 - _bricks[i].getPosition().y * 20.0f - 10.0f) < MIN_WALL_DISTANCE && (m_position.x + m_dim.x / 2 - _bricks[i].getPosition().x * 20.0f - 10.0f) > 0)
		{
			return;
		}
	}
	m_position += glm::vec2(-m_speed, 0.0f);
	return;


}
void Character::moveRIGHT(std::vector<BrickExplode> &_bricks, std::vector<glm::vec2> _brickFixedPos)
{
	if (m_position.x > 663.0f)
		return;
	

	for (int i = 0; i < _brickFixedPos.size(); i++) 
	{
		if (abs(m_position.x + m_dim.x / 2 - _brickFixedPos[i].x * 20.0f - 10.0f) < MIN_WALL_DISTANCE && abs(m_position.y + m_dim.y / 2 - _brickFixedPos[i].y * 20.0f - 10.0f) < MIN_WALL_DISTANCE && (m_position.x + m_dim.x / 2 - _brickFixedPos[i].x * 20.0f - 10.0f) < 0)
		{
			return;
		}

	}

	for (int i = 0; i < _bricks.size(); i++)
	{
		// 10.0f is half of brick dim
		if (_bricks[i].getVisibility() && abs(m_position.x + m_dim.x / 2 - _bricks[i].getPosition().x * 20.0f - 10.0f) < MIN_WALL_DISTANCE && abs(m_position.y + m_dim.y / 2 - _bricks[i].getPosition().y * 20.0f - 10.0f) < MIN_WALL_DISTANCE && (m_position.x + m_dim.x / 2 - _bricks[i].getPosition().x * 20.0f - 10.0f) < 0)
		{
			return;
		}
	}
	m_position += glm::vec2(m_speed, 0.0f);
	return;
}