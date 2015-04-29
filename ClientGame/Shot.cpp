#include "stdafx.h"
#include "Shot.h"


Shot::Shot(float X, float Y, float angle, Vector2 shooterVel, float r, float g, float b){
	//initialize shot position and speed
	shotVel.X = angle;
	shotVel.Y = 0.15;
	shotPosition.X = X;
	shotPosition.Y = Y;
	//shotVel += shooterVel;
	//setup shot
	this->SetPosition(shotPosition);
	if (angle > 0.01){
		this->SetRotation(-20);
	}
	else if (angle < -0.01){
		this->SetRotation(20);
	}
	this->SetSize(0.5f);
	this->SetColor(r, g, b, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/Shot.png", 0, GL_CLAMP, GL_NEAREST, false);
	this->SetLayer(2); //player shots layer
	this->SetGroupIndex(-1);
	this->SetIsSensor(true);
	SetDensity(0.05f);
	InitPhysics();
	ApplyLinearImpulse(shotVel, Vector2::Zero);
	theWorld.Add(this);
	Tag("Bullet");
	Tag("Friendly");
	SetName("PlayerShot");
	theSwitchboard.SubscribeTo(this, "CollisionStartWith" + GetName());
}

void Shot::Update(float dt){
	timeAlive += dt;
	//shots stay alive for 5 seconds
	if (timeAlive >= 1.6 || shouldDie){
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
		if (!collider->IsDestroyed()){
			if (collider->GetName() != "PlayerShip" && !collider->IsTagged("Bullet") && !collider->IsTagged("Stage") && !collider->IsTagged("Friendly")){
				shouldDie = true;
			}
		}
	}
}