#include "stdafx.h"
#include "BigAsteroid.h"


BigAsteroid::BigAsteroid(float X, float Y, float velX, float velY){
	health = 10;
	//initialize asteroid position and speed
	velocity.X = velX;
	velocity.Y = velY;
	position.X = X;
	position.Y = Y;
	//asteroidVel += shooterVel;
	//setup asteroid
	this->SetPosition(position);
	this->SetSize(3.0f);
	this->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/AsteroidHit.png", 1, GL_CLAMP, GL_NEAREST, false);
	this->SetSprite("Resources/Images/Asteroid.png", 0, GL_CLAMP, GL_NEAREST, false);
	this->SetLayer(1); //enemy layer
	this->SetGroupIndex(-2);
	SetDensity(10.0f);
	SetShapeType(PhysicsActor::SHAPETYPE_CIRCLE);
	InitPhysics();
	ApplyAngularImpulse(0.25);
	direction = velocity;
	//direction.Normalize();
	//direction *= 20;
	ApplyLinearImpulse(direction, Vector2::Zero);
	theWorld.Add(this);
	Tag("Asteroid");
	SetName("BigAsteroid");
	theSwitchboard.SubscribeTo(this, "CollisionStartWith" + GetName());
}

void BigAsteroid::Update(float dt){
	if (health > 0){
		//update asteroid position according to its velocity
		position += velocity;
		//move asteroid
		this->SetRotation(this->GetRotation() + 1);

		animateHit();
	}
	if (health <= 0 && !dying){
		dying = true;
		this->LoadSpriteFrames("Resources/Images/Explosion_001.png", GL_CLAMP, GL_NEAREST);
		this->PlaySpriteAnimation(0.1, SAT_Loop, 0, 6, "explode");
	}
	if (dying){
		switch (dyingFrame){
		case 0:
			SetSpriteFrame(0);
			break;
		case 3:
			SetSpriteFrame(1);
			break;
		case 6:
			SetSpriteFrame(2);
			break;
		case 9:
			SetSpriteFrame(3);
			break;
		case 12:
			SetSpriteFrame(4);
			break;
		case 15:
			SetSpriteFrame(5);
			break;
		case 18:
			SetSpriteFrame(6);
			break;
		case 21:
			Destroy();
			break;
		}
		dyingFrame++;
	}

}

void BigAsteroid::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == "CollisionStartWith" + GetName())
	{
		PhysicsActor* collider = (PhysicsActor*)message->GetSender();
		if (!isHit)
		{
			health -= 1;
			isHit = true;
		}
	}
}
void BigAsteroid::animateHit(){
	if (isHit){
		if (currentHitFrame <= 4){
			this->SetSpriteFrame(1);
			currentHitFrame++;
		}
		else{
			this->SetSpriteFrame(0);
			isHit = false;
			currentHitFrame = 0;
		}
	}
}