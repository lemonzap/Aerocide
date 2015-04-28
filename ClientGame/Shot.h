
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>

class Shot : public PhysicsActor{
public:
	Shot(float X, float Y, Vector2 shooterVel);

	void Update(float dt);
	void Shot::ReceiveMessage(Message *message);
private:
	Vector2 shotPosition;
	Vector2 shotVel;
	float timeAlive = 0;
};