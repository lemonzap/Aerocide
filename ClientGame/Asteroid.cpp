#include "stdafx.h"
#include "Asteroid.h"


Asteroid::Asteroid(float X, float Y, float velX, float velY){
	health = 3;
	//initialize asteroid position and speed
	velocity.X = velX;
	velocity.Y = velY;
	position.X = X;
	position.Y = Y;
	//asteroidVel += shooterVel;
	//setup asteroid
	this->SetPosition(position);
	this->SetSize(0.75f);
	this->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/Asteroid.png", 0, GL_CLAMP, GL_NEAREST, false);
	this->SetLayer(1); //enemy layer
	SetDensity(5.0f);
	SetShapeType(PhysicsActor::SHAPETYPE_CIRCLE);
	InitPhysics();
	ApplyAngularImpulse(0.25);
	direction = velocity;
	//direction.Normalize();
	//direction *= 20;
	ApplyLinearImpulse(direction, Vector2::Zero);
	theWorld.Add(this);
	Tag("Asteroid");
	SetName("Asteroid");
	theSwitchboard.SubscribeTo(this, "CollisionStartWith" + GetName());
}

void Asteroid::Update(float dt){

	if (health > 0){
		//update asteroid position according to its velocity
		position += velocity;
		//move asteroid
		this->SetRotation(this->GetRotation() + 1);

		animateHit();
	}
	else{
		Destroy();
	}

}

void Asteroid::ReceiveMessage(Message *message)
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

void Asteroid::animateHit(){
	if (isHit){
		if (currentHitFrame <= 4){
			this->SetSprite("Resources/Images/AsteroidHit.png", 0, GL_CLAMP, GL_NEAREST, false);
			currentHitFrame++;
		}
		else if (currentHitFrame <= 9){
			this->SetSprite("Resources/Images/Asteroid.png", 0, GL_CLAMP, GL_NEAREST, false);
			currentHitFrame++;
		}
		else if (currentHitFrame <= 14){
			this->SetSprite("Resources/Images/AsteroidHit.png", 0, GL_CLAMP, GL_NEAREST, false);
			currentHitFrame++;
		}
		else if (currentHitFrame <= 19){
			this->SetSprite("Resources/Images/Asteroid.png", 0, GL_CLAMP, GL_NEAREST, false);
			currentHitFrame++;
		}
		else if (currentHitFrame <= 24){
			this->SetSprite("Resources/Images/AsteroidHit.png", 0, GL_CLAMP, GL_NEAREST, false);
			currentHitFrame++;
		}
		else{
			this->SetSprite("Resources/Images/Asteroid.png", 0, GL_CLAMP, GL_NEAREST, false);
			isHit = false;
			currentHitFrame = 0;
		}
	}
}