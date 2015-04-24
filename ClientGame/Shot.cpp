#include "stdafx.h"
#include "Shot.h"


Shot::Shot(float X, float Y, Vector2 shooterVel){
	//initialize shot position and speed
	shotVel.X = 0;
	shotVel.Y = 0.15;
	shotPosition.X = X;
	shotPosition.Y = Y;
	//shotVel += shooterVel;
	//setup shot
	this->SetSize(0.5f);
	this->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/Shot.png", 0, GL_CLAMP, GL_NEAREST, false);
	this->SetLayer(2); //player shots layer
	theWorld.Add(this);
	this->SetPosition(shotPosition);
}

void Shot::Update(float dt){
	timeAlive += dt;
	//shots stay alive for 5 seconds
	if (timeAlive >= 5){
		Destroy();
	}
	else{
		//update shot position according to its velocity
		shotPosition += shotVel;
		//move shot
		this->SetPosition(shotPosition);
	}

}