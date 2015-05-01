
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include "Shot.h"
#include "HealthBar.h"

class Player : public PhysicsActor{ //inheriting from physics actor
public:
	friend class HealthBar;
	Player();
	void Shoot(float X, float Y, Vector2 shooterVel, bool pierce);
	void BeamShoot(float X, float Y, Vector2 shooterVel, bool pierce);
	void TripleShoot(float X, float Y, Vector2 shooterVel, bool pierce);
	void animateHit();

	void Update(float dt);
	void ReceiveMessage(Message *message);
	int score;
	int health;
private:
	HealthBar* healthBar;
	Vector2 position;
	Vector2 velocity;
	Vector2 direction;
	int powerLevel = 1; // Should be 1 for base level
	int framesSinceLastShot = 0;
	int shotCooldownFrames;
	int currentHitFrame = 0;
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