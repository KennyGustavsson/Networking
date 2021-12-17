#pragma once
#include "Vector2.h"

class Particle
{
public:
	Particle(vector2 pos, vector2 vel, int life, vector2 screenSize);
	void Update();
	void Render();
	bool isDead();

	void SetPos(vector2 pos);
	void SetVel(vector2 vel);
	void SetLifeTime(int life);

	bool hide = false;

private:
	vector2 _pos;
	vector2 _vel;
	vector2 _screenSize;
	float _baseSpeed = 100.0f;

	int _endTime = 0;
};

