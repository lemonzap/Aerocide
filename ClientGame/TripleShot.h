
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>

class TripleShot : public PhysicsActor{
public:
	TripleShot(float X, float Y);

	void Update(float dt);
	void ReceiveMessage(Message *message);
private:
	Vector2 position;
	Vector2 velocity;
	Vector2 direction;
	int animFrame = 0;
};