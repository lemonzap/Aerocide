
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include "HealthPoint.h"

class HealthBar : public Actor{
public:
	HealthBar(int initialHealth, float x, float y);
	void removeHealth(int amount);
	void addHealth(int amount);
private:
	Vector2 position;
	int health;
	HealthPoint* healthPoints[16];
};