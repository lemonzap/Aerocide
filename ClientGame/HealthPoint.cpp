#include "stdafx.h"
#include "healthBar.h"


HealthPoint::HealthPoint(float x, float y){
	this->SetSize(0.85, 0.85f);
	position = Vector2(x, y);
	this->SetPosition(position);
	this->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/HealthPoint.png", 0, GL_CLAMP, GL_NEAREST, false);
	this->SetLayer(5); //Health Point layer
	theWorld.Add(this);
}