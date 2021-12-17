#include "Particle.h"
#include "Engine.h"

Particle::Particle(vector2 pos, vector2 vel, int life, vector2 screenSize) : _pos(pos), _vel(vel)
{
	_endTime = engElapsedTime() + life;
	_screenSize = screenSize;
}

void Particle::Update()
{
	_pos += _vel * engDeltaTime() * _baseSpeed;
}

/// <summary>
/// Renders the particle
/// </summary>
/// <param name="renderer"></param>
void Particle::Render()
{
	if (isDead() || hide)
		return;

	engFillRect((int)_pos.x - 1, (int)_pos.y - 1, 2, 2);
	
}

bool Particle::isDead()
{
	return (engElapsedTime() > _endTime);
}

void Particle::SetPos(vector2 pos)
{
	_pos = pos;
}

void Particle::SetVel(vector2 vel)
{
	_vel = vel;
}

void Particle::SetLifeTime(int life)
{
	_endTime = engElapsedTime() + life;
}
