#include "stdafx.h"
#include "Shot.h"


Shot::Shot(float X, float Y, float angle, Vector2 shooterVel, float r, float g, float b, bool newIsSuper, bool pierce){
	isSuper = newIsSuper;
	shouldPierce = pierce;
	//initialize shot position and speed
	shotVel.X = angle;
	shotVel.Y = 0.15;
	shotPosition.X = X;
	shotPosition.Y = Y;
	//shotVel += shooterVel;
	//setup shot
	this->SetPosition(shotPosition);
	if (angle > 0.01){
		this->SetRotation(-20); //setting rotation
	}
	else if (angle < -0.01){
		this->SetRotation(20);
	}
	this->SetSize(0.5f);
	if (!isSuper){
		this->SetColor(r, g, b, 1); //(white and opaque so the texture comes through fully)
	}
	else{
		this->SetColor(0.8, 0, 1, 1); //(white and opaque so the texture comes through fully)
	}
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/Shot.png", 0, GL_CLAMP, GL_NEAREST, false);
	this->SetLayer(2); //player shots layer
	this->SetGroupIndex(-1);
	this->SetIsSensor(true);
	SetDensity(0.05f); //setting density
	InitPhysics();
	ApplyLinearImpulse(shotVel, Vector2::Zero);
	theWorld.Add(this);
	Tag("bullet");
	Tag("Friendly"); //tagging the shot
	SetName("PlayerShot");
	theSwitchboard.SubscribeTo(this, "CollisionStartWith" + GetName());
}

void Shot::Update(float dt){
	timeAlive += dt;
	//shots stay alive for 1.6 seconds
	if (timeAlive >= 1.6 || shouldDie){
		Destroy();
	}
	else{
		if (isSuper){
			switch (superFrame){
			case 1:
				this->SetColor(1, 0, 0, 1);
				superFrame = 2;
				break;
			case 2:
				this->SetColor(1, 1, 0, 1);
				superFrame = 3;
				break;
			case 3:
				this->SetColor(0, 1, 0, 1);
				superFrame = 4;
				break;
			case 4:
				this->SetColor(0, 0, 1, 1);
				superFrame = 5;
				break;
			case 5:
				this->SetColor(0.8, 0, 1, 1);
				superFrame = 1;
				break;
			}
		}
	}

}

void Shot::ReceiveMessage(Message *message) //coliding with something
{
	try{
		if (!shouldDie){
			if (message->GetMessageName() == "CollisionStartWith" + GetName())
			{
				if (message->GetSender() != NULL){
					PhysicsActor* collider = (PhysicsActor*)message->GetSender();
					if (!collider->IsDestroyed() && !shouldDie && !IsDestroyed()){
						if (!collider->IsTagged("bullet") && !collider->IsTagged("Stage") && !collider->IsTagged("Friendly")){
							if (!shouldPierce){
								shouldDie = true;
							}
							theSwitchboard.UnsubscribeFrom(this, "CollisionStartWith" + GetName());
						}
					}
				}
			}
		}
	}
	catch (int e){
		std::cout << "shot Error" << std::endl;
	}
}