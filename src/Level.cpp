#include "Level.h"
#include "Engine.h"

Level level;

Level::Level()
{
	int width = 800;
	int height = 600;
	mapWidth = 40;
	mapHeight = 30;

	map += L"########################################";
	map += L"#......................................#";
	map += L"#......................................#";
	map += L"#..#####........................#####..#";
	map += L"#..#................................#..#";
	map += L"#..#................................#..#";
	map += L"#..#................................#..#";
	map += L"#..#................................#..#";
	map += L"#......................................#";
	map += L"#...........###.........###............#";
	map += L"#......................................#";
	map += L"#......................................#";
	map += L"#......................................#";
	map += L"#......................................#";
	map += L"#......................................#";
	map += L"#......................................#";
	map += L"#......................................#";
	map += L"#......................................#";
	map += L"#......................................#";
	map += L"#......................................#";
	map += L"#..............#########...............#";
	map += L"#......................................#";
	map += L"#..#................................#..#";
	map += L"#..#................................#..#";
	map += L"#..#................................#..#";
	map += L"#..#................................#..#";
	map += L"#..#####........................#####..#";
	map += L"#......................................#";
	map += L"#......................................#";
	map += L"########################################";
}

void Level::draw()
{
	engSetColor(0xFFFFFFFF);

	auto GetTile = [&](int x, int y)
	{
		if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
			return map[y * mapWidth + x];
		else
			return L' ';
	};

	for (int x = -1; x < mapWidth; x++)
	{
		for (int y = -1; y < mapHeight; y++) 
		{
			wchar_t tile = GetTile(x, y);

			if (tile == L'#')
			{
				engFillRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
			}
		}
	}
}

rayHit Level::raycastAgainstLevel(vector2 start, vector2 dir, float length)
{
	start = start / CELL_SIZE;
	length = length / CELL_SIZE;

	vector2 rayUnitStepSize;
	rayUnitStepSize.x = abs(1.0f / dir.x);
	rayUnitStepSize.y = abs(1.0f / dir.y);

	int mapCheckX = start.x;
	int mapCheckY = start.y;
	vector2 rayLenght1D;

	int stepX;
	int stepY;

	if (dir.x < 0) 
	{
		stepX = -1;
		rayLenght1D.x = (start.x - float(mapCheckX)) * rayUnitStepSize.x;
	}
	else 
	{
		stepX = 1;
		rayLenght1D.x = (float(mapCheckX + 1) - start.x) * rayUnitStepSize.x;
	}

	if (dir.y < 0)
	{
		stepY = -1;
		rayLenght1D.y = (start.y - float(mapCheckY)) * rayUnitStepSize.y;
	}
	else
	{
		stepY = 1;
		rayLenght1D.y = (float(mapCheckY + 1) - start.y) * rayUnitStepSize.y;
	}

	bool bTileFound = false;
	float distance = 0.0f;
	while (!bTileFound && distance < length)
	{
		if (rayLenght1D.x < rayLenght1D.y) 
		{
			mapCheckX += stepX;
			distance = rayLenght1D.x;
			rayLenght1D.x += rayUnitStepSize.x;
		}
		else 
		{
			mapCheckY += stepY;
			distance = rayLenght1D.y;
			rayLenght1D.y += rayUnitStepSize.y;
		}

		if (mapCheckX >= 0 && mapCheckX < mapWidth && mapCheckY >= 0 && mapCheckY < mapHeight)
		{
			if (map[mapCheckY * mapWidth + mapCheckX] == L'#')
			{
				bTileFound = true;
			}
		}
	}

	vector2 intersection;
	if (bTileFound) 
	{
		intersection = start + dir * distance;
		intersection.x *= CELL_SIZE;
		intersection.y *= CELL_SIZE;
	}

	rayHit hit;
	hit.hit = bTileFound;
	hit.hitPos = intersection;

	return hit;
}

vector2 Level::checkAgainstLevelCollision(vector2 currentPosition, vector2 potentialPosition, float radius)
{
	vector2 outPos;
	outPos.x = potentialPosition.x;
	outPos.y = potentialPosition.y;

	auto GetTile = [&](int x, int y)
	{
		if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
			return map[y * mapWidth + x];
		else
			return L' ';
	};

	// Shitty AABB collision
	for (int x = -1; x < mapWidth; x++)
	{
		for (int y = -1; y < mapHeight; y++)
		{
			wchar_t tile = GetTile(x, y);

			if (tile == L'#')
			{
				int CellX = x * CELL_SIZE;
				int CellY = y * CELL_SIZE;

				
				if (potentialPosition.x - radius < CellX + CELL_SIZE &&
					potentialPosition.x + radius > CellX &&
					potentialPosition.y - radius < CellY + CELL_SIZE &&
					potentialPosition.y + radius > CellY
					)
				{
					vector2 direction = potentialPosition - currentPosition;
					direction = direction.Normalize();

					engPrint("%f, %f", direction.x, direction.y);

					// DOWN
					if (direction.y > 0 && direction.x == 0 && potentialPosition.y + radius > CellY)
					{
						outPos.y = CellY - radius;
					}
					// UP
					else if (direction.y < 0 && direction.x == 0 && potentialPosition.y - radius < CellY + CELL_SIZE)
					{
						outPos.y = CellY + CELL_SIZE + radius;
					}

					// RIGHT
					if (direction.x > 0 && direction.y == 0 && potentialPosition.x - radius < CellX)
					{
						outPos.x = CellX - radius;
					}
					// LEFT
					else if (direction.x < 0 && direction.y == 0 && potentialPosition.x + radius > CellX)
					{
						outPos.x = CellX + CELL_SIZE + radius;
					}

					// DOWN & RIGHT
					if (direction.x > 0 && direction.y > 0)
					{
						outPos.y = currentPosition.y;
						outPos.x = currentPosition.x;
					}
					// DOWN & LEFT
					else if (direction.x < 0 && direction.y > 0)
					{
						outPos.y = currentPosition.y;
						outPos.x = currentPosition.x;

					}
					// UP & RIGHT
					else if (direction.x > 0 && direction.y < 0)
					{
						outPos.y = currentPosition.y;
						outPos.x = currentPosition.x;
					}
					// UP & LEFT
					else if (direction.x < 0 && direction.y < 0)
					{
						outPos.y = currentPosition.y;
						outPos.x = currentPosition.x;
					}

					return outPos;
				}
			}
		}
	}

	return outPos;
}

bool Level::checkAgainstLevelCollision(vector2 currentPosition, float radius)
{
	auto GetTile = [&](int x, int y)
	{
		if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
			return map[y * mapWidth + x];
		else
			return L' ';
	};

	// Shitty AABB collision
	for (int x = -1; x < mapWidth; x++)
	{
		for (int y = -1; y < mapHeight; y++)
		{
			wchar_t tile = GetTile(x, y);

			if (tile == L'#')
			{
				int CellX = x * CELL_SIZE;
				int CellY = y * CELL_SIZE;


				if (currentPosition.x - radius < CellX + CELL_SIZE &&
					currentPosition.x + radius > CellX &&
					currentPosition.y - radius < CellY + CELL_SIZE &&
					currentPosition.y + radius > CellY
					)
				{
					return true;
				}
			}
		}
	}

	return false;
}
