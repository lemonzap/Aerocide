#include "stdafx.h"
#include "Player.h"


Player::Player(){
	health = 10;
	//setup player

	this->SetSize(1.0f);
	this->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/Ship.png", 0, GL_CLAMP, GL_NEAREST, false);
	this->SetLayer(3); //player layer
	SetDensity(0.05f);
	InitPhysics();
	theWorld.Add(this);
	Tag("Ship");
	SetName("PlayerShip");
	position.X = 0.0f;
	position.Y = 0.0f;
	this->SetPosition(position);
	velocity.X = 0.00f;
	velocity.Y = 0.00f;
	theSwitchboard.SubscribeTo(this, "CollisionStartWith" + GetName());
}

void Player::Update(float dt){

	if (health > 0){
		//update player position according to its velocity
		position += velocity;
		//move
		//set vectors to zero each frame
		direction.X = 0;
		direction.Y = 0;
		//handle movement inputs (up: y = 1, down: y = -1, left: x = -1, right: x = 1)
		if (theInput.IsKeyDown('w')){
			direction.Y += 1;
		}
		if (theInput.IsKeyDown('s')){
			direction.Y -= 1;
		}
		if (theInput.IsKeyDown('a')){
			direction.X -= 1;
		}
		if (theInput.IsKeyDown('d')){
			direction.X += 1;
		}
		//if vector is not (0,0) then normalize it (normalizing a zero vector produces (1,0) for some reason)
		if (direction != Vector2::Zero){
			direction.Normalize();
		}
		//this->ApplyLinearImpulse(-velocity, Vector2::Zero);
		//multiply normalized vector components by the players maximum speed (variable needed)
		velocity.X = direction.X * 10.0f;
		velocity.Y = direction.Y * 10.0f;

		GetBody()->SetLinearVelocity(b2Vec2(velocity.X, velocity.Y));

		//if space is pressed and shooting is off cooldown then shoot
		if (theInput.IsKeyDown(' ') && framesSinceLastShot >= shotCooldownFrames){
			framesSinceLastShot = 1;
			Shoot(this->GetPosition().X, this->GetPosition().Y + 0.0f, velocity);
		}
		else{
			framesSinceLastShot++;
		}
		//stop moving if the player reaches any of the edges of the screen
		if (GetPosition().X < -9.5 && this->GetBody()->GetLinearVelocity().x < 0){
			this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
		}
		else if (GetPosition().X > 9.5 && this->GetBody()->GetLinearVelocity().x > 0){
			this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
		}
		if (GetPosition().Y < -9.5 && this->GetBody()->GetLinearVelocity().y < 0){
			this->GetBody()->SetLinearVelocity(b2Vec2(this->GetBody()->GetLinearVelocity().x, 0));
		}
		else if (GetPosition().Y > 9.5 && this->GetBody()->GetLinearVelocity().y > 0){
			this->GetBody()->SetLinearVelocity(b2Vec2(this->GetBody()->GetLinearVelocity().x, 0));
		}
		animateHit();
	}
	else{
		Destroy();
	}

}

void Player::ReceiveMessage(Message *message)
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

void Player::Shoot(float X, float Y, Vector2 shooterVel){
	new Shot(X, Y + 0.8, shooterVel);
}

void Player::animateHit(){
	if (isHit){
		if (currentHitFrame <= 4){
			this->SetSprite("Resources/Images/ShipHit.png", 0, GL_CLAMP, GL_NEAREST, false);
			currentHitFrame++;
		}
		else if (currentHitFrame <= 9){
			this->SetSprite("Resources/Images/Ship.png", 0, GL_CLAMP, GL_NEAREST, false);
			currentHitFrame++;
		}
		else if (currentHitFrame <= 14){
			this->SetSprite("Resources/Images/ShipHit.png", 0, GL_CLAMP, GL_NEAREST, false);
			currentHitFrame++;
		}
		else if (currentHitFrame <= 19){
			this->SetSprite("Resources/Images/Ship.png", 0, GL_CLAMP, GL_NEAREST, false);
			currentHitFrame++;
		}
		else if (currentHitFrame <= 24){
			this->SetSprite("Resources/Images/ShipHit.png", 0, GL_CLAMP, GL_NEAREST, false);
			currentHitFrame++;
		}
		else{
			this->SetSprite("Resources/Images/Ship.png", 0, GL_CLAMP, GL_NEAREST, false);
			isHit = false;
			currentHitFrame = 0;
		}
	}
}