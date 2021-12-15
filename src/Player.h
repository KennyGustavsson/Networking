#pragma once
#include "2DMesh.h"
#include "Laser.h"

#define PLAYER_MAX 20
#define PLAYER_NAME_MAX 15

const float playerSpeed = 180.f;
const float playerErrorCorrectionStrength = 1.5f;
const float playerRadius = 16;

class Player
{
public:
	Player();

	Mesh mesh;

	int id = -1;
	bool alive = false;
	bool shield = false;
	float x = 0;
	float y = 0;

	int inputX = 0;
	int inputY = 0;

	float errorX = 0.f;
	float errorY = 0.f;

	float shieldTimer = 0.0f;
	float shieldTime = 2.0f;
	float lastShieldTime = 0.0f;
	float shieldCooldown = 5.0f;

	float lastFireTime = 0.f;

	char name[PLAYER_NAME_MAX + 1];

	void netReceivePosition(float newX, float newY);

	void spawn(int id, int spawnX, int spawnY);
	void activateShield();
	bool canBeShielded();
	void destroy();

	bool hasControl();
	void update();

	void draw();
};

extern Player players[PLAYER_MAX];

#if CLIENT
extern int possessedPlayerId;
#endif