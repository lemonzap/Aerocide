
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>

class Shot : public PhysicsActor{ //inheriting from the physics actor class
public:
	Shot(float X, float Y, float angle, Vector2 shooterVel, float r, float g, float b); //Accepts position, angle of the shot and the shooter

	void Update(float dt);
	void Shot::ReceiveMessage(Message *message);
private:
	Vector2 shotPosition;
	Vector2 shotVel;
	float timeAlive = 0;
	bool shouldDie = false;
};