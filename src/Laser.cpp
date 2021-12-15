#include "Laser.h"
#include "Engine.h"
#include "Player.h"
#include "Server.h"

Laser laser;

Laser::Laser()
{
	_position = vector2(400, 300);
	_startDirection = vector2(0, 1);
}

void Laser::update()
{
	if (_fireLaser) {
		_accumulator += engDeltaTime() / _speed;
		_rot = lerp(0, TAU, _accumulator);

		_rotatedDirection.x = _startDirection.x * cos(_rot) - _startDirection.y * sin(_rot);
		_rotatedDirection.y = _startDirection.x * sin(_rot) + _startDirection.y * cos(_rot);

		rayHit hit;
		hit = level.raycastAgainstLevel(_position, _rotatedDirection, _range);
		if (hit.hit)
		{
			_laserEndPos = hit.hitPos;
		}
		else
		{
			_laserEndPos = _rotatedDirection * _range;
		}

		vector2 samplePoint;
		float accumulator = 0;

#if SERVER
		while (accumulator < 1.0f) 
		{
			samplePoint.x = lerp(_position.x, _laserEndPos.x, accumulator);
			samplePoint.y = lerp(_position.y, _laserEndPos.y, accumulator);

			for (auto& p : players)
			{
				if (p.alive && !p.shield) 
				{
					float diffX = samplePoint.x - p.x;
					float diffY = samplePoint.y - p.y;
					float distSqr = (diffX * diffX + diffY * diffY);
					float radiusSqr = playerRadius * playerRadius;

					if (distSqr < radiusSqr)
					{
						serverKickUser(p.id);
					}
				}
			}

			accumulator += 0.05f;
		}
#endif

		if (_accumulator >= 1.0f)
		{
			_fireLaser = false;
			_accumulator = 0;
		}
	}
	else if (_varmup) 
	{
		if (_varmupTimer >= _varmupTime) 
		{
			_varmup = false;
			_fireLaser = true;
			_varmupTimer = 0.0f;
		}

		_varmupTimer += engDeltaTime();
	}
}

void Laser::draw()
{
	// Draw mesh
	if (_varmup) 
	{
		engSetColor(0xFF0000FF);
	}
	else 
	{
		engSetColor(0xFFFFFFFF);
	}

	engFillRect(_position.x - 16, _position.y - 16, 32, 32);

	// Draw laser
	if (_fireLaser) 
	{
		engSetColor(0xFF0000FF);
		drawLine(_position, _laserEndPos);
	}
}

void Laser::fireLaser(vector2 direction)
{
	_startDirection = direction;
	_varmup = true;
	_accumulator = 0.0f;
	_varmupTimer = 0.0f;
}
