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
	m_texId[NOTSHOOTING] = Bengine::ResourceManager::getTexture(m_filePaths[m_person]).id;
	//m_texId[SHOOTING] =Bengine::ResourceManager::getTexture(m_filePaths2[m_person]).id;
	m_speed = speed;
	m_levelData = levelData;
	m_state = NOTSHOOTING;
	m_choice = choice;
	m_score = -1;
}

void Character::init()
{
}

Character::~Character()
{
}

void Character::setData(float x, float y /*, float health, int score*/)
{
	m_position.x = x;
	m_position.y = y;
	//m_health = health;
	//m_score = score;
}


void Character::setBrickToPop(int index) {
	std::cout << " doing "<< index << std::endl;
	m_score = index;
}

std::string Character::getData()
{
	std::string result = std::to_string(m_position.x) + " " + std::to_string(m_position.y) + "|" + std::to_string(m_health) + "|"  +std::to_string(m_score) + "|";
	return result;
}

void Character::draw(Bengine::SpriteBatch& spriteBatch)
{
	spriteBatch.draw(glm::vec4(m_position.x, m_position.y, m_dim.x, m_dim.y), m_uv, m_texId[m_state], 0.0f, m_color);
}

void Character::respawn()
{
	m_health = 200;
	int x = rand() % RESPAWN_PLACES;
	m_position = respawnPosition[x];
}


bool Character::damageTaken(int damage)
{
	m_health -= damage;
	if (m_health <= 0)
	{
		std::cout << m_name << " DEAD" << std::endl;
		respawn();
		return true;
	}
	return false;
}


void Character::moveUP(std::vector<BrickExplode> &_bricks)
{
	if (m_position.y > 930.0f)
		return;
	/*
	if ((m_levelData[floor(m_position.x / (float)TILE_WIDTH)][ceil((m_position.y + m_dim.y) / (float)TILE_WIDTH)] != '.') ||
		(m_levelData[floor((m_position.x + m_dim.x) / (float)TILE_WIDTH)][ceil((m_position.y + m_dim.y) / (float)TILE_WIDTH)] != '.'))		//wall above somewhere
	{
		int distance = ((int)(m_position.y + m_dim.y)) % TILE_WIDTH;
		if ((TILE_WIDTH - distance) < MIN_WALL_DISTANCE)
			return; //without updating the position, as the player cannot move any closer than the min distance 
	}
	m_position += glm::vec2(0.0f, m_speed);
	return;*/
	for (int i = 0; i < _bricks.size(); i++)
	{
		// 20.0f is tile width , 10.0f is half of brick dim
		if (_bricks[i].getVisibility() && abs(m_position.x + m_dim.x / 2 - _bricks[i].getPosition().x * 20.0f - 10.0f) < MIN_WALL_DISTANCE && abs(m_position.y + m_dim.y / 2 - _bricks[i].getPosition().y * 20.0f - 10.0f) < MIN_WALL_DISTANCE && (m_position.y + m_dim.y / 2 - _bricks[i].getPosition().y * 20.0f - 10.0f) < 0)
		{
			std::cout << "returning " << std::endl;
			return;
		}
	}
	m_position += glm::vec2(0.0f, m_speed);
	return;
}
void Character::moveDOWN(std::vector<BrickExplode> &_bricks)
{
	if (m_position.y < 23.0f)                   // down border screen coordinates
		return;
	/*
	if ((m_levelData[floor(m_position.x / (float)TILE_WIDTH)][floor((m_position.y) / (float)TILE_WIDTH) - 1] != '.') ||
		(m_levelData[floor((m_position.x + m_dim.x) / (float)TILE_WIDTH)][floor((m_position.y) / (float)TILE_WIDTH) - 1] != '.')) //wall below somewhere
	{
		int distance = ((int)(m_position.y)) % TILE_WIDTH;
		if (distance < MIN_WALL_DISTANCE)
			return; //without updating the position, as the player cannot move any closer than the min distance 
	}
	m_position += glm::vec2(0.0f, -m_speed);
	return;*/

	for (int i = 0; i < _bricks.size(); i++)
	{
		if (_bricks[i].getVisibility() && abs(m_position.x + m_dim.x / 2 - _bricks[i].getPosition().x * 20.0f - 10.0f) < MIN_WALL_DISTANCE && abs(m_position.y + m_dim.y / 2 - _bricks[i].getPosition().y * 20.0f - 10.0f) < MIN_WALL_DISTANCE && (m_position.y + m_dim.y / 2 - _bricks[i].getPosition().y * 20.0f - 10.0f) > 0)
		{
			std::cout << "returning " << std::endl;
			return;
		}
	}
	m_position += glm::vec2(0.0f, -m_speed);
	return;
}
void Character::moveLEFT(std::vector<BrickExplode> &_bricks)
{
	if (m_position.x < 23.0f)
		return;
	/*if ((m_levelData[floor(m_position.x / (float)TILE_WIDTH) - 1][floor((m_position.y + m_dim.y) / (float)TILE_WIDTH)] != '.') ||
		(m_levelData[floor((m_position.x) / (float)TILE_WIDTH) - 1][floor((m_position.y) / (float)TILE_WIDTH)] != '.'))	//wall on the left somehwere
	{
		int distance = ((int)(m_position.x)) % TILE_WIDTH;
		if (distance < MIN_WALL_DISTANCE)
			return; //without updating the position, as the player cannot move any closer than the min distance 
	}
	m_position += glm::vec2(-m_speed, 0.0f);
	return;*/

	for (int i = 0; i < _bricks.size(); i++)
	{
		if (_bricks[i].getVisibility() && abs(m_position.x + m_dim.x / 2 - _bricks[i].getPosition().x * 20.0f - 10.0f) < MIN_WALL_DISTANCE && abs(m_position.y + m_dim.y / 2 - _bricks[i].getPosition().y * 20.0f - 10.0f) < MIN_WALL_DISTANCE && (m_position.x + m_dim.x / 2 - _bricks[i].getPosition().x * 20.0f - 10.0f) > 0)
		{
			std::cout << "returning " << std::endl;
			return;
		}
	}
	m_position += glm::vec2(-m_speed, 0.0f);
	return;


}
void Character::moveRIGHT(std::vector<BrickExplode> &_bricks)
{
	if (m_position.x > 663.0f)
		return;
	//int temp1x = ceil((m_position.x + m_dim.x) / (float)TILE_WIDTH);
	//int temp1y = floor((m_position.y + m_dim.y) / (float)TILE_WIDTH);
	//int temp2x = ceil((m_position.x + m_dim.x) / (float)TILE_WIDTH);
	//int temp2y = floor((m_position.y) / (float)TILE_WIDTH);
	//if ((m_levelData[temp1x ][temp1y] != '.') || /*(m_levelData[temp1x+1][temp1y] != '.') || (m_levelData[temp1x + 2][temp1y] != '.')||*/
	//	(m_levelData[temp2x][temp2y] != '.') /*|| (m_levelData[temp2x+1][temp2y] != '.')*/)	//wall on the right
	/*{
		int distance = ((int)(m_position.x + m_dim.x)) % TILE_WIDTH;
		std::cout << "distance " << TILE_WIDTH - distance << " " << m_position.x<< std::endl;
		if ((TILE_WIDTH - distance) < MIN_WALL_DISTANCE)
			return;	//without updating the position, as the player cannot move any closer than the min distance 
	}
	else
	{
		std::cout << "found dot at" << temp1x << " " << temp1y<< " " << temp2x << " " << temp2y;
	}
	std::cout << " moving distance " << TILE_WIDTH - ((int)(m_position.x + m_dim.x)) % TILE_WIDTH << " " << m_position.x << std::endl;
	m_position += glm::vec2(m_speed, 0.0f);
	return;
	*/

	for (int i = 0; i < _bricks.size(); i++)
	{
		// 10.0f is half of brick dim
		if (_bricks[i].getVisibility() && abs(m_position.x + m_dim.x / 2 - _bricks[i].getPosition().x * 20.0f - 10.0f) < MIN_WALL_DISTANCE && abs(m_position.y + m_dim.y / 2 - _bricks[i].getPosition().y * 20.0f - 10.0f) < MIN_WALL_DISTANCE && (m_position.x + m_dim.x / 2 - _bricks[i].getPosition().x * 20.0f - 10.0f) < 0)
		{
			std::cout << "returning " << std::endl;
			return;
		}
	}
	m_position += glm::vec2(m_speed, 0.0f);
	return;
}