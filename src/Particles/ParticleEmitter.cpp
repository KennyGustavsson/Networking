#include "ParticleEmitter.h"
#include "Engine.h"

/// <summary>
/// Emitter that follows a position and rotation.
/// </summary>
/// <param name="maxParticles"></param>
/// <param name="Pos"></param>
/// <param name="Rot"></param>
FollowingParticleEmitter::FollowingParticleEmitter(int maxParticles, vector2* Pos, vector2* Rot, vector2* screenSize)
{
	_pos = Pos;
	_rot = Rot;
	_maxParticle = maxParticles;
	_particles = std::vector<Particle*>(_maxParticle);
	_screenSize = screenSize;

	_particles = std::vector<Particle*>(_maxParticle);
	for (int i = 0; i < _maxParticle; i++)
	{

		vector2 rotation = vector2(0, 0);
		if (_invertRot)
		{
			rotation.x = -(*_rot).x;
			rotation.y = -(*_rot).y;
		}
		else
		{
			rotation = *_rot;
		}

		vector2 pos = { (*_pos).x + RandomSigned(_randomPos.x), (*_pos).y + RandomSigned(_randomPos.y) };
		float life = _lifeLength + Random(_randomLifeLength);
		vector2 rot = { 0, 0 };

		if (_useRot)
		{
			rot = { rotation.x + RandomSigned(_randomRot.x), rotation.y + RandomSigned(_randomRot.y) };
			_particles[i] = new Particle(pos, rot, life, _screenSize);
		}
		else
		{
			rot = { RandomSigned(_randomRot.x), RandomSigned(_randomRot.y) };
			_particles[i] = new Particle(pos, rot, life, _screenSize);
		}

		_particles[i]->hide = true;
	}
}

FollowingParticleEmitter::FollowingParticleEmitter(int maxParticles, vector2* Pos, float spawnTime, float randomColor, vector2* screenSize)
{
	_pos = Pos;
	_maxParticle = maxParticles;
	_particles = std::vector<Particle*>(_maxParticle);
	_screenSize = screenSize;
	_spawnTime = spawnTime;
	_useRot = false;

	_particles = std::vector<Particle*>(_maxParticle);
	for (int i = 0; i < _maxParticle; i++)
	{
		vector2 pos = { (*_pos).x + RandomSigned(_randomPos.x), (*_pos).y + RandomSigned(_randomPos.y) };
		float life = _lifeLength + Random(_randomLifeLength);
		vector2 rot = { 0, 0 };

		rot = { RandomSigned(_randomRot.x), RandomSigned(_randomRot.y) };
		_particles[i] = new Particle(pos, rot, life, _screenSize);

		_particles[i]->hide = true;
	}
}

FollowingParticleEmitter::~FollowingParticleEmitter()
{
	for (int i = 0; i < _maxParticle; i++)
	{
		delete _particles[i];
	}
}

void FollowingParticleEmitter::Emitt()
{
	_timer += engDeltaTime();

	for (int i = 0; i < _maxParticle; i++) {
		if (_particles[i]->isDead()) {
			if (_timer > _spawnTime || _spawnTime == 0.0f) {
				_timer = 0;

				vector2 rotation = vector2(0, 0);
				if (_useRot)
				{
					if (_invertRot)
					{
						rotation.x = -(*_rot).x;
						rotation.y = -(*_rot).y;
					}
					else
					{
						rotation = *_rot;
					}
				}

				vector2 pos = { (*_pos).x + RandomSigned(_randomPos.x), (*_pos).y + RandomSigned(_randomPos.y) };
				float life = _lifeLength + Random(_randomLifeLength);
				vector2 rot = { 0, 0 };

				if (_useRot)
				{
					rot = { rotation.x + RandomSigned(_randomRot.x), rotation.y + RandomSigned(_randomRot.y) };

					_particles[i]->SetPos(pos);
					_particles[i]->SetVel(rot);
					_particles[i]->SetLifeTime(life);
					_particles[i]->hide = false;
				}
				else
				{
					rot = { RandomSigned(_randomRot.x), RandomSigned(_randomRot.y) };

					_particles[i]->SetPos(pos);
					_particles[i]->SetVel(rot);
					_particles[i]->SetLifeTime(life);
					_particles[i]->hide = false;
				}
			}
		}
	}
}

void FollowingParticleEmitter::Update()
{
	for (int i = 0; i < _maxParticle; i++)
	{
		if (!_particles[i]->isDead())
			_particles[i]->Update();
	}
}

void FollowingParticleEmitter::Render(SDL_Renderer* renderer)
{
	for (int i = 0; i < _maxParticle; i++)
	{
		if (!_particles[i]->isDead())
			_particles[i]->Render();
	}
}
