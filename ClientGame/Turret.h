
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include "TurretShot.h"
#include "TripleShot.h"
#include "BeamShot.h"
#include "PierceShot.h"
#include "Health.h"

class Turret : public PhysicsActor{
public:
	Turret(float X, float Y, float angle, char TurSize, PhysicsActor* newStage);
	void animateHit();
	void Shoot(float X, float Y, Vector2 direction);

	void Update(float dt);
	void ReceiveMessage(Message *message);
private:
	Vector2 position;
	Vector2 velocity;
	Vector2 direction;
	PhysicsActor* stage;
	int health;
	char size = 'l';
	int framesSinceLastShot = 0;
	int shotCooldownFrames;
	int currentHitFrame = 0;
	bool isHit = false;
	bool dying = false;
	int dyingFrame = 0;
	Vector2 stagePosition;
	int rotation = 1;
	AngelSoundHandle explode;
};