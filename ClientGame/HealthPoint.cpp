
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include "Shot.h"

class HealthPoint : public PhysicsActor{
public:
	HealthPoint();
private:
	Vector2 position;
};