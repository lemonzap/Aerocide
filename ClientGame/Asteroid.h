
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include "Health.h"

class Asteroid : public PhysicsActor{
public:
	Asteroid(float X, float Y, float velX, float velY);
	void animateHit();

	void Update(float dt);
	void ReceiveMessage(Message *message);
private:
	Vector2 position;
	Vector2 velocity;
	Vector2 direction;
	int health;
	int currentHitFrame = 0;
	bool isHit = false;
	bool dying = false;
	int dyingFrame = 0;
	AngelSoundHandle explode;
};