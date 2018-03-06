#pragma once
#include <string>
#include <vector>
#include <Bengine/SpriteBatch.h>

const int TILE_WIDTH = 20;


class Level
{
public:
	Level();
	Level(const std::string &fileName, int screenWidth, int screenHeight);
	~Level();

	void draw();


	const std::vector<std::string> getLevelData() const { return _levelData; }


	

private:
	
	std::vector<std::string> _levelData;

	Bengine::SpriteBatch _spriteBatch;
	GLuint grassTexid;

};

