
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include "Shot.h"

class HealthPoint : public Actor{
public:
	HealthPoint(float x, float y);
private:
	Vector2 position;
};