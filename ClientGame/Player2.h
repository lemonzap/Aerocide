
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include "Shot.h"
#include "HealthBar.h"

class Player2 : public PhysicsActor{
public:
	friend class HealthBar;
	Player2();
	void Shoot(float X, float Y, Vector2 shooterVel, bool pierce);
	void BeamShoot(float X, float Y, Vector2 shooterVel, bool pierce);
	void TripleShoot(float X, float Y, Vector2 shooterVel, bool pierce);
	void animateHit();

	void Update(float dt);
	void ReceiveMessage(Message *message);
	int score;
private:
	HealthBar* healthBar;
	Vector2 position;
	Vector2 velocity;
	Vector2 direction;
	int powerLevel = 1; // Should be 1 for base level
	int framesSinceLastShot = 0;
	int shotCooldownFrames;
	int health;
	int currentHitFrame = 0;
	int currentUpgradeFrame = 0;
	bool isHit = false;
	int dyingFrame = 0;
	bool dying = false;
	AngelSampleHandle shootSound;
	AngelSoundHandle explode;
	AngelSoundHandle healthSound;
	AngelSoundHandle upgradeSound;
	AngelSoundHandle downgradeSound;
	AngelSoundHandle hitSound;
};