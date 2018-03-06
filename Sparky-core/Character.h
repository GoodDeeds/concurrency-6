#pragma once
#include <glm/glm.hpp>
#include <Bengine/SpriteBatch.h>
#include <string>
#include <vector>
//#include "Gun.h"
#include "Bullet.h"
#include "level.h"

#include "BrickExplode.h"


const int MIN_WALL_DISTANCE = 20;
const int RESPAWN_PLACES = 4;

enum State
{
	NOTSHOOTING = 0,
	SHOOTING = 1
};
/*
enum Person
{
	HARRY = 0,
	RON = 1,
	HERMOINE = 2,
	GINNY = 3,
	MALFOY = 4,
	LUNA = 5
};*/


	class Character
	{
	public:
		Character(std::string name, glm::vec2 pos, int person, glm::vec2 dim, int speed, int choice , const std::vector<std::string>& levelData);
		~Character();
		void init();
		void draw(Bengine::SpriteBatch& spriteBatch);
		void update();
		/*
		void addGun(Gun gun);
		void nextGun();
		void selectGun(unsigned int n);
		bool shoot(const glm::vec2& direction, std::vector<Bullet>& bullets);
		void stopShoot();
		*/
		void moveUP(std::vector<BrickExplode> &_bricks, std::vector<glm::vec2> _brickFixedPos);
		void moveDOWN(std::vector<BrickExplode> &_bricks, std::vector<glm::vec2> _brickFixedPos);
		void moveRIGHT(std::vector<BrickExplode> &_bricks , std::vector<glm::vec2> _brickFixedPos);
		void moveLEFT(std::vector<BrickExplode> &_bricks, std::vector<glm::vec2> _brickFixedPos);
		bool damageTaken(int damage);

		void setBrickToPop(int index);
		//getters
		
		glm::vec2 getPosition() { return (m_position + glm::vec2(m_dim.x / 2, m_dim.y / 2)); }	//dimension added to get position of the centre of the character
		float getHealth() { return m_health; }
		glm::vec2 getDim() { return m_dim; }
		float getMana() { return m_mana; }
		
		std::string getData();
		//std::string getNameScore() { return m_name + "\t\t\t" + std::to_string(m_score); }
		//setters

		void setData(float x, float y/*, float health, int score*/);
		//void increaseScore();
	private:
		void respawn();
		glm::vec2 m_position, m_dim;
		glm::vec4 m_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		Bengine::Color m_color = { 255,255,255,255 };
		std::string m_name;
		int m_person;
		GLuint m_texId[2];	//stores the ids of the textures. non-shooting mode, shooting mode
		int m_state;	//stores if it is in shooting mode or non shooting mode
		int m_speed;
		float m_health = 200;
		float m_mana = 100;
		int m_score = 0;
		int m_choice;
		std::vector<std::string> m_levelData;
		/*
		std::vector<Gun> m_guns;
		int m_currentGunIndex;
		*/
		std::string m_filePaths[6] = { "../Sparky-core/Textures/Characters/black.png","../Sparky-core/Textures/Characters/silver.png","../Sparky-core/Textures/Characters/bronze.png","../Sparky-core/Textures/Characters/grey.png","../Sparky-core/Textures/Characters/archer.png","../Sparky-core/Textures/Characters/wizard.png" };
		//std::string m_filePaths2[6] = { "../Sparky-core/Textures/harryPotter/Harry/harry2.png","../Sparky-core/Textures/harryPotter/Ron/ron2.png","../Sparky-core/Textures/harryPotter/Hermoine/hermoine2.png","../Sparky-core/Textures/harryPotter/Ginny/ginny2.png","../Sparky-core/Textures/harryPotter/Malfoy/malfoy2.png","../Sparky-core/Textures/harryPotter/Luna/luna2.png" };
		//glm::vec2 respawnPosition[RESPAWN_PLACES] = { glm::vec2(130.0f,30.0f),glm::vec2(150.0f,30.0f), glm::vec2(150.0f,50.0f), glm::vec2(170.0f,70.0f) };
		
		glm::vec2 respawnPosition[RESPAWN_PLACES] = { glm::vec2(130.0f,30.0f),glm::vec2(150.0f,30.0f), glm::vec2(150.0f,50.0f), glm::vec2(170.0f,70.0f) };
	};

