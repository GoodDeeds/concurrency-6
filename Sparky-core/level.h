#pragma once
#include <string>
#include <vector>
#include <Bengine/SpriteBatch.h>

const int TILE_WIDTH = 240;

class Level
{
public:
	Level(const std::string &fileName, int screenWidth, int screenHeight);
	~Level();

	void draw();

	//glm::vec2 getStartPlayerPos() const { return _playerStartPos; }

	//const std::vector<glm::vec2>& getZombieStartPos() const { return _zombieStartPositions; }

	const std::vector<std::string> getLevelData() const { return _levelData; }

	//const int getNumHumans() const { return _numHumans; }

	//const int getWidth() const { return _levelData[0].size(); }
	//const int getHeight() const { return _levelData.size(); }
private:
	std::vector<std::string> _levelData;
	//int _numHumans;
	Bengine::SpriteBatch _spriteBatch;
	GLuint grassTexid;
	//glm::vec2 _playerStartPos;
	//std::vector<glm::vec2> _zombieStartPositions;
};

