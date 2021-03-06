#include "stdafx.h"
#include "Turret.h"


Turret::Turret(float X, float Y, float angle, char TurSize, PhysicsActor* newStage){
	if (TurSize == 's') //size small
	{
		size = 's';
		health = 5;
		this->SetSize(0.833f, 1.666f);
		SetDensity(0.05f);
		shotCooldownFrames = 30;
	}

	else if (TurSize == 'l') //size large
	{
		shotCooldownFrames = 2;
		SetDensity(0.001f);
		size = 'l';
		health = 60;
		this->SetSize(1.75f, 3.5f);
	}

	stage = newStage;
	//initialize turret position and speed
	position.X = X;
	position.Y = Y;
	//std::cout << cos(angle*0.01745) << ", " << sin(angle*0.01745) << std::endl;
	this->direction = Vector2(cos((angle + 90)*0.01745), sin((angle + 90)*0.01745));
	//turretVel += shooterVel;
	//setup turret
	this->SetPosition(position);
	this->SetRotation(angle);
	this->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/TurretTopHit.png", 1, GL_CLAMP, GL_NEAREST, false);
	this->SetSprite("Resources/Images/TurretTop.png", 0, GL_CLAMP, GL_NEAREST, false);
	explode = theSound.LoadSample("Resources/Sounds/Explosion.wav", false);
	this->SetLayer(2); //enemy layer
	this->SetGroupIndex(-2);
	this->SetIsSensor(true);
	SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	InitPhysics();
	this->GetBody()->SetLinearVelocity(b2Vec2(0, -6));
	theWorld.Add(this);
	Tag("Turret");
	SetName("Turret");
	theSwitchboard.SubscribeTo(this, "CollisionStartWith" + GetName());
}

void Turret::Update(float dt){ //updating the turret
	if (health > 0){
		this->GetBody()->SetLinearVelocity(stage->GetBody()->GetLinearVelocity());
		this->direction = Vector2(cos((GetRotation() + 90)*0.01745), sin((GetRotation() + 90)*0.01745));
		if (framesSinceLastShot >= shotCooldownFrames){
			framesSinceLastShot = 1;
			Shoot(this->GetPosition().X, this->GetPosition().Y, direction);
			ApplyAngularImpulse(rotation*0.05);
			rotation *= -1;
		}
		else{
			framesSinceLastShot++;
		}

		animateHit();
	}
	if (health <= 0 && !dying){ //turret dies
		dying = true;
		theSound.PlaySound(explode, 1.0f, false, 0);
		if (size == 'l'){ //different drop rate for larger turret
			float rand = MathUtil::RandomFloat();
			if (rand > 0.5f){
				new TripleShot(this->GetPosition().X, this->GetPosition().Y);
			}
			else if (rand <= 0.5f){
				new BeamShot(this->GetPosition().X, this->GetPosition().Y);
			}
			if (MathUtil::RandomFloat() > 0.5f){
				new Health(this->GetPosition().X, this->GetPosition().Y);
			}
		}
		else{
			float rand = MathUtil::RandomFloat();
			if (rand > 0.8f){
				new TripleShot(this->GetPosition().X, this->GetPosition().Y);
			}
			else if (rand > 0.6f){
				new BeamShot(this->GetPosition().X, this->GetPosition().Y);
			}
			else if (rand > 0.4f){
				new PierceShot(this->GetPosition().X, this->GetPosition().Y);
			}
		}
		
		this->LoadSpriteFrames("Resources/Images/Explosion_001.png", GL_CLAMP, GL_NEAREST);
		this->PlaySpriteAnimation(0.1, SAT_Loop, 0, 6, "explode");
	}
	if (dying){ //dying animation
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
			this->_deleteMe = true; //This is making sure that the turret destroys itself if dead.
			break;			//This was done to fix a bug if the turret died while coliding with the stage boundaries
		case 21:
			Destroy();
			break;
		}
		dyingFrame++;
	}

}

void Turret::ReceiveMessage(Message *message) //coliding with something
{
	try{
		if (health > 0){
			if (message->GetMessageName() == "CollisionStartWith" + GetName())
			{
				PhysicsActor* collider = (PhysicsActor*)message->GetSender();
				if (!collider->IsDestroyed() && !IsDestroyed()){
					if (!isHit && !collider->IsTagged("Asteroid") && !collider->IsTagged("Stage") && !collider->IsTagged("EnemyBullet"))
					{
						health -= 1;
						isHit = true; //takes damage from player shots
					}
				}
			}
		}
	}
	catch (int e){
		std::cout << "Turret Error" << std::endl;
	}
}

void Turret::Shoot(float X, float Y, Vector2 newDirection){ //makes a turret shot for every new shot
	new TurretShot(X, Y, newDirection, size, stage->GetBody()->GetLinearVelocity().y);
}

void Turret::animateHit(){ //using the hit animation
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