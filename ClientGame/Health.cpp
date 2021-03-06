#include "stdafx.h"
#include "Health.h"


Health::Health(float X, float Y){
	//initialize health position and speed
	position.X = X;
	position.Y = Y;
	//healthVel += shooterVel;
	//setup health
	this->SetPosition(position);
	this->SetSize(1.0f);
	this->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->LoadSpriteFrames("Resources/Images/Health_001.png", GL_CLAMP, GL_NEAREST);
	this->SetLayer(2); //enemy layer
	this->SetGroupIndex(-2);
	this->SetIsSensor(true);
	SetDensity(5.0f);
	SetShapeType(PhysicsActor::SHAPETYPE_CIRCLE);
	InitPhysics();
	//ApplyAngularImpulse(1);
	//direction.Normalize();
	//direction *= 20;
	direction = Vector2(Vector2(MathUtil::RandomFloat() * 20, MathUtil::RandomFloat() * 20));
	ApplyLinearImpulse(direction, Vector2::Zero);
	theWorld.Add(this);
	Tag("Health");
	Tag("Friendly");
	SetName("Health");
	theSwitchboard.SubscribeTo(this, "CollisionStartWith" + GetName());
}

void Health::Update(float dt){

	timeAlive += dt;
	//shots stay alive for 5 seconds
	if (timeAlive >= 7){
		Destroy();
	}

	//update health position according to its velocity
	position += velocity;
	//move health
	this->SetRotation(this->GetRotation() + 1);

	if (GetPosition().X < -9.5 && this->GetBody()->GetLinearVelocity().x < 0){
		this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
		direction.X *= -1;
		ApplyLinearImpulse(Vector2(direction.X, 0), Vector2::Zero);
	}
	else if (GetPosition().X > 9.5 && this->GetBody()->GetLinearVelocity().x > 0){
		this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
		direction.X *= -1;
		ApplyLinearImpulse(Vector2(direction.X, 0), Vector2::Zero);
	}
	if (GetPosition().Y < -9.5 && this->GetBody()->GetLinearVelocity().y < 0){
		this->GetBody()->SetLinearVelocity(b2Vec2(this->GetBody()->GetLinearVelocity().x, 0));
		direction.Y *= -1;
		ApplyLinearImpulse(Vector2(0, direction.Y), Vector2::Zero);
	}
	else if (GetPosition().Y > 9.5 && this->GetBody()->GetLinearVelocity().y > 0){
		this->GetBody()->SetLinearVelocity(b2Vec2(this->GetBody()->GetLinearVelocity().x, 0));
		direction.Y *= -1;
		ApplyLinearImpulse(Vector2(0, direction.Y), Vector2::Zero);
	}

	switch (animFrame){
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
		SetSpriteFrame(7);
		break;
	case 24:
		SetSpriteFrame(8);
		break;
	}
	animFrame++;
	if (animFrame == 27){
		animFrame = 0;
	}

}

void Health::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == "CollisionStartWith" + GetName())
	{
		PhysicsActor* collider = (PhysicsActor*)message->GetSender();
		if (!collider->IsDestroyed()){
			if (collider->IsTagged("Ship")){
				Destroy();
			}
		}
	}
}