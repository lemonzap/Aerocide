#include "stdafx.h"
#include "Turret.h"


Turret::Turret(float X, float Y, float angle, char TurSize, PhysicsActor* newStage){
	if (TurSize == 's')
	{
		size = 's';
		health = 5;
		this->SetSize(0.833f, 1.666f);
	}

	else if (TurSize == 'l')
	{
		size = 'l';
		health = 60;
		this->SetSize(1.75f, 3.5f);
	}
	shotCooldownFrames = 30;
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
	this->SetLayer(2); //enemy layer
	this->SetIsSensor(true);
	SetDensity(0.05f);
	SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	InitPhysics();
	this->GetBody()->SetLinearVelocity(b2Vec2(0, -6));
	theWorld.Add(this);
	Tag("Turret");
	SetName("Turret");
	theSwitchboard.SubscribeTo(this, "CollisionStartWith" + GetName());
}

void Turret::Update(float dt){
	if (health > 0){
		this->GetBody()->SetLinearVelocity(stage->GetBody()->GetLinearVelocity());
		this->direction = Vector2(cos((GetRotation() + 90)*0.01745), sin((GetRotation() + 90)*0.01745));
		if (framesSinceLastShot >= shotCooldownFrames){
			framesSinceLastShot = 1;
			Shoot(this->GetPosition().X, this->GetPosition().Y, this->direction);
			ApplyAngularImpulse(rotation*0.05);
			rotation *= -1;
		}
		else{
			framesSinceLastShot++;
		}

		animateHit();
	}
	if (health <= 0 && !dying){
		dying = true;
		if (MathUtil::RandomFloat() > 0.5f){
		new TripleShot(this->GetPosition().X, this->GetPosition().Y);
		}
		else{
			new BeamShot(this->GetPosition().X, this->GetPosition().Y);
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

void Turret::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == "CollisionStartWith" + GetName())
	{
		PhysicsActor* collider = (PhysicsActor*)message->GetSender();
		if (!collider->IsDestroyed()){
		if (!isHit && !collider->IsTagged("Asteroid") && !collider->IsTagged("Stage") && !collider->IsTagged("EnemyBullet"))
		{
			health -= 1;
			isHit = true;
		}
	}
}
}

void Turret::Shoot(float X, float Y, Vector2 newDirection){
	new TurretShot(X, Y, newDirection, size);
}

void Turret::animateHit(){
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