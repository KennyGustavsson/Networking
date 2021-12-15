#pragma once
#include "vector2.h"
#include "Particle.h"
#include <iostream>
#include <vector>

class FollowingParticleEmitter
{
public:
	FollowingParticleEmitter(int maxParticles, vector2* Pos, vector2* Rot, vector2* screenSize);
	FollowingParticleEmitter(int maxParticles, vector2* Pos, float spawnTime, float randomColor, vector2* screenSize);
	~FollowingParticleEmitter();

	void Emitt();
	void Update();
	void Render(SDL_Renderer* renderer);

private:
	float RandomSigned(float x)
	{
		float a = x + x;
		return (float(rand()) / float((RAND_MAX)) * a) - x;
	}

	float Random(float x)
	{
		return (float(rand()) / float((RAND_MAX)) * x);
	}

	std::vector<Particle*> _particles;
	vector2* _screenSize;

	vector2* _pos;
	vector2* _rot;

	int _maxParticle;

	float _spawnTime = 0.001f;
	float _timer = 0;

	bool _invertRot = true;
	bool _useRot = true;

	vector2 _randomPos = { 0.2f, 0.2f };
	vector2 _randomRot = { 0.4f, 0.4f };

	float _lifeLength = 0.5f;
	float _randomLifeLength = 4.0f;
};

