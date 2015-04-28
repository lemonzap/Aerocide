
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include "Shot.h"

class Player : public PhysicsActor{
public:
	Player();
	void Shoot(float X, float Y, Vector2 shooterVel);
	void animateHit();

	void Update(float dt);
	void ReceiveMessage(Message *message);
private:
	Vector2 position;
	Vector2 velocity;
	Vector2 direction;
	int framesSinceLastShot = 0;
	int shotCooldownFrames = 15;
	int health;
	int currentHitFrame = 0;
	bool isHit = false;
};