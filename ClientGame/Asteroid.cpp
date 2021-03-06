#include "stdafx.h"
#include "Asteroid.h"


Asteroid::Asteroid(float X, float Y, float velX, float velY){
	health = 2;
	//initialize asteroid position and speed
	velocity.X = velX;
	velocity.Y = velY;
	position.X = X;
	position.Y = Y;
	//asteroidVel += shooterVel;
	//setup asteroid
	this->SetPosition(position); //setting initial position
	this->SetSize(0.75f);
	this->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/AsteroidHit.png", 1, GL_CLAMP, GL_NEAREST, false);
	this->SetSprite("Resources/Images/Asteroid.png", 0, GL_CLAMP, GL_NEAREST, false); //getting textures
	explode = theSound.LoadSample("Resources/Sounds/Explosion.wav", false);
	this->SetLayer(1); //enemy layer
	this->SetGroupIndex(-2);
	SetDensity(5.0f);
	SetShapeType(PhysicsActor::SHAPETYPE_CIRCLE);
	InitPhysics();
	ApplyAngularImpulse(MathUtil::RandomFloatInRange(-1,1)*1);
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

		animateHit();

		if (GetPosition().Y < -70){
			Destroy();
		}
	}
	if (health <= 0 && !dying){
		dying = true;
		theSound.PlaySound(explode, 1.0f, false, 0);
		if (MathUtil::RandomFloat() < 0.025f){
			new Health(this->GetPosition().X, this->GetPosition().Y);
		}
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

void Asteroid::ReceiveMessage(Message *message) //coliding with something
{
	try{
		if (message->GetMessageName() == "CollisionStartWith" + GetName())
		{
			PhysicsActor* collider = (PhysicsActor*)message->GetSender();
			if (!collider->IsDestroyed()){
				if (!isHit)
				{
					health -= 1;
					isHit = true;
					if (collider->IsTagged("Ship")){
						health -= 3;
					}
				}
				
			}
		}
	}
	catch (int e){
		std::cout << "Asteroid Error" << std::endl;
	}
}

void Asteroid::animateHit(){ //animating the hit
	if (isHit){
		if (currentHitFrame <= 2){
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