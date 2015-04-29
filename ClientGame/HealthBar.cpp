
#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include "HealthPoint.h"

class HealthBar : public Actor{
public:
	HealthBar();
	void removeHealth(int amount);
	void addHealth(int amount);
private:
	Vector2 position;
	int health;
};