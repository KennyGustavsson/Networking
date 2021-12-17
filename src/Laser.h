#pragma once

#include "Level.h"
#include "Vector2.h"
#include "Particles/ParticleEmitter.h"

class Laser
{
public:
	Laser();
	void update();
	void draw();
	void fireLaser(vector2 direction);
	int getDirection() const;
	void setDirection(int dir);

private:
	FollowingParticleEmitter _emitter;

	vector2 _position;
	vector2 _laserEndPos;

	vector2 _startDirection;
	vector2 _rotatedDirection;

	float _range = 500;
	float _speed = 8;
	float _accumulator = 0;
	float _rot = 0;

	float _varmupTimer = 0.0f;
	float _varmupTime = 2.0f;

	int _direction = 1;

	bool _fireLaser = false;
	bool _varmup = false;
};

extern Laser laser;
