#pragma once

#include "stdafx.h"
#include <vector>
#include <string>

class TurretShot : public PhysicsActor{
public:
	TurretShot(float X, float Y, Vector2 newDirection, char size, float newStageSpeed);

	void Update(float dt);
	void TurretShot::ReceiveMessage(Message *message);
private:
	Vector2 shotPosition;
	Vector2 shotVel;
	Vector2 direction;
	float timeAlive = 0;
	bool shouldDie = false;
	float stageSpeed;
};