
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>

class Shot : public Actor{
public:
	Shot(float X, float Y, Vector2 shooterVel);

	void Update(float dt);
private:
	Vector2 shotPosition;
	Vector2 shotVel;
	float timeAlive = 0;
};