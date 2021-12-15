#include "Player.h"
#include "Client.h"
#include "Engine.h"
#include "MessageType.h"
#include "Network.h"
#include <cmath>
#include "Projectile.h"

#define clamp(a, min, max) (a < min ? min : (a > max ? max : a))

Player players[PLAYER_MAX];
#if CLIENT
int possessedPlayerId = -1;
#endif

Player::Player()
{
	mesh.LoadFromObjFile("assets/alien.obj");
}

void Player::netReceivePosition(float newX, float newY)
{
	newX = clamp(newX, 0.f + playerRadius, 800.f - playerRadius);
	newY = clamp(newY, 0.f + playerRadius, 600.f - playerRadius);

	errorX = newX - x;
	errorY = newY - y;
}

void Player::spawn(int id, int spawnX, int spawnY)
{
	this->id = id;
	alive = true;
	/*x = (float) spawnX;
	y = (float) spawnY;*/

	x = 400;
	y = 500;
}

void Player::activateShield()
{
	shield = true;
	lastShieldTime = engElapsedTime();
}

bool Player::canBeShielded()
{
	return lastShieldTime + shieldCooldown < engElapsedTime();
}

void Player::destroy()
{
	alive = false;
}

bool Player::hasControl()
{
#if SERVER
	return false;
#else
	return id == possessedPlayerId;
#endif
}

void Player::update()
{
#if CLIENT
	if(hasControl())
	{
		int frameInputX = 0;
		int frameInputY = 0;

		if (engKeyDown(Key::A)) frameInputX -= 1;
		if (engKeyDown(Key::D)) frameInputX += 1;
		if (engKeyDown(Key::W)) frameInputY -= 1;
		if (engKeyDown(Key::S)) frameInputY += 1;

		if (frameInputX != inputX || frameInputY != inputY)
		{
			NetMessage msg;
			msg.write<MessageType>(MessageType::PlayerInput);
			msg.write<int>(id);
			msg.write<float>(x);
			msg.write<float>(y);

			msg.write<char>(frameInputX);
			msg.write<char>(frameInputY);

			clientSend(msg);
			msg.free();
		}

		inputX = frameInputX;
		inputY = frameInputY;

		if(engKeyPressed(Key::Space))
		{
			NetMessage msg;
			msg.write<MessageType>(MessageType::PlayerRequestFire);
			clientSend(msg);
			msg.free();
		}

		if (engKeyDown(Key::E)) 
		{
			NetMessage msg;
			msg.write<MessageType>(MessageType::PlayerRequestShield);
			clientSend(msg);
			msg.free();
		}
	}
#endif

	if(!hasControl())
	{
		float errorDeltaX = errorX * playerErrorCorrectionStrength * engDeltaTime();
		float errorDeltaY = errorY * playerErrorCorrectionStrength * engDeltaTime();

		x += errorDeltaX;
		y += errorDeltaY;
		errorX -= errorDeltaX;
		errorY -= errorDeltaY;
	}

	float newX = x + inputX * playerSpeed * engDeltaTime();
	float newY = y + inputY * playerSpeed * engDeltaTime();

	vector2 current(x, y);
	vector2 potential(newX, newY);
	vector2 collisionChecked;

	collisionChecked = level.checkAgainstLevelCollision(current, potential, playerRadius);

	x = collisionChecked.x;
	y = collisionChecked.y;

	x = clamp(x, 0.f + playerRadius, 800.f - playerRadius);
	y = clamp(y, 0.f + playerRadius, 600.f - playerRadius);

	if (shield) 
	{
		if (shieldTimer >= shieldTime) 
		{
			shield = false;
			shieldTimer = 0.0f;
		}

		shieldTimer += engDeltaTime();
	}
}

void Player::draw()
{
	engSetColor(0xDEADBEEF);

	vector2 pos(x, y);
#if CLIENT
	if (hasControl()) 
	{
		if (canBeShielded())
		{
			engSetColor(0xADFFBEFF);
		}
		else
		{
			engSetColor(0xADDEBEEF);
		}
	}

	auto aim = level.raycastAgainstLevel(pos, vector2(inputX, inputY), 800);
	if (aim.hit)
	{
		drawCircle(aim.hitPos, 10, 16);
	}
#endif
	engText((int)x - playerRadius, (int)y - playerRadius - 16, name);

	if (shield) 
	{
		drawCircle(pos, playerRadius, 32);
	}

	drawModel(mesh, pos, 0);
}
