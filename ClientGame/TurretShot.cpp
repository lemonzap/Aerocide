#include "stdafx.h"
#include "TurretShot.h"


TurretShot::TurretShot(float X, float Y, Vector2 newDirection, char TurSize, float newStageSpeed){
	//initialize shot position and speed
	shotPosition.X = X;
	shotPosition.Y = Y;
	direction = newDirection;
	stageSpeed = newStageSpeed;
	//shotVel += shooterVel;
	//setup shot
	this->SetSize(0.15f);
	if (TurSize == 's')
	{
		SetDensity(0.05f);
		this->SetSize(0.15f);
	}
	else if (TurSize == 'l') {
		SetDensity(0.01f);
		this->SetSize(.35f);
	}
	this->SetPosition(shotPosition);
	this->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/TurretShot.png", 0, GL_CLAMP, GL_NEAREST, false);
	this->SetLayer(1); //player shots layer
	this->SetIsSensor(true);
	SetShapeType(PhysicsActor::SHAPETYPE_CIRCLE);
	InitPhysics();
	this->GetBody()->SetLinearVelocity(b2Vec2(0, stageSpeed));
	ApplyLinearImpulse(direction*0.01, Vector2::Zero);
	theWorld.Add(this);
	Tag("Bullet");
	Tag("EnemyBullet");
	SetName("TurretShot");
	theSwitchboard.SubscribeTo(this, "CollisionStartWith" + GetName());
}

void TurretShot::Update(float dt){
	timeAlive += dt;
	//shots stay alive for 5 seconds
	if (timeAlive >= 2){
		Destroy();
	}
}

void TurretShot::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == "CollisionStartWith" + GetName())
	{
		PhysicsActor* collider = (PhysicsActor*)message->GetSender();
		if (!collider->IsDestroyed()){
			if (!collider->IsTagged("Asteroid") && !collider->IsTagged("Bullet") && !collider->IsTagged("Stage") && !collider->IsTagged("Turret") && !collider->IsTagged("Friendly")){
				Destroy();
			}
		}
	}
}