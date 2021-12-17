#pragma once
#include "Vector2.h"
#include <vector>
#include <string>

#define CELL_SIZE 20

struct rayHit 
{
	bool hit = false;
	vector2 hitPos;
};

class Level
{
public:
	Level();
	void draw();
	rayHit raycastAgainstLevel(vector2 start, vector2 dir, float length);
	vector2 checkAgainstLevelCollision(vector2 currentPosition, vector2 potentialPosition, float radius);
	bool checkAgainstLevelCollision(vector2 currentPosition, float radius);

	std::wstring map;
	int mapWidth;
	int mapHeight;

private:
	wchar_t GetTile(int x, int y);
};

extern Level level;