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
	this->SetPosition(shotPosition);
	this->SetSize(0.5f);
	this->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/Shot.png", 0, GL_CLAMP, GL_NEAREST, false);
	this->SetLayer(2); //player shots layer
	SetDensity(0.05f);
	InitPhysics();
	ApplyLinearImpulse(shotVel, Vector2::Zero);
	theWorld.Add(this);
	Tag("Bullet");
	SetName("PlayerShot");
	theSwitchboard.SubscribeTo(this, "CollisionStartWith" + GetName());
}

void Shot::Update(float dt){
	timeAlive += dt;
	//shots stay alive for 5 seconds
	if (timeAlive >= 5){
		Destroy();
	}
	else{
		
	}

}

void Shot::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == "CollisionStartWith" + GetName())
	{
		PhysicsActor* collider = (PhysicsActor*)message->GetSender();
		if (true)
		{
			Destroy();
		}
	}
}