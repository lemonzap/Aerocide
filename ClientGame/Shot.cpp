#include "stdafx.h"
#include "Shot.h"


Shot::Shot(float X, float Y, Vector2 shooterVel){
	shotVel.X = 0;
	shotVel.Y = 0.15;
	shotPosition.X = X;
	shotPosition.Y = Y;
	//shotVel += shooterVel;
	this->SetSize(0.5f);
	this->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/Shot.png", 0, GL_CLAMP, GL_NEAREST, false);
	this->SetLayer(2);
	theWorld.Add(this);
	this->SetPosition(shotPosition);
}

void Shot::Update(float dt){
	timeAlive += dt;
	if (timeAlive >= 5){
		Destroy();
	}
	else{
		shotPosition += shotVel;
		this->SetPosition(shotPosition);
	}

}