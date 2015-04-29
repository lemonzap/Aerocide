#pragma once

#include "stdafx.h"
#include <vector>
#include <string>

class HugeAsteroid : public PhysicsActor{
public:
	HugeAsteroid(float X, float Y, float velX, float velY);
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
};