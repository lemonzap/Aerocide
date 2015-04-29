#include "stdafx.h"
#include "Player2.h"


Player2::Player2(){
	health = 16;
	shotCooldownFrames = 15;
	//setup player2
	healthBar = new HealthBar(health, 0, 7.8);
	this->SetSize(1.0f);
	this->SetPosition(5, 0);
	this->SetColor(0.5, 1, 0.5, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/ShipHit.png", 1, GL_CLAMP, GL_NEAREST, false);
	this->SetSprite("Resources/Images/Ship.png", 0, GL_CLAMP, GL_NEAREST, false);
	shootSound = theSound.LoadSample("Resources/Sounds/Shoot.wav", false);
	explode = theSound.LoadSample("Resources/Sounds/Explosion.wav", false);
	healthSound = theSound.LoadSample("Resources/Sounds/Health.wav", false);
	upgradeSound = theSound.LoadSample("Resources/Sounds/Upgrade.wav", false);
	downgradeSound = theSound.LoadSample("Resources/Sounds/Downgrade.wav", false);
	hitSound = theSound.LoadSample("Resources/Sounds/HitSound.wav", false);
	this->SetLayer(3); //player layer
	this->SetFixedRotation(true);
	this->SetGroupIndex(-1);
	SetDensity(0.05f);
	SetShapeType(PhysicsActor::SHAPETYPE_CIRCLE);
	InitPhysics();
	theWorld.Add(this);
	Tag("Ship");
	SetName("Player2Ship");
	position.X = 0.0f;
	position.Y = 0.0f;
	velocity.X = 0.00f;
	velocity.Y = 0.00f;
	theSwitchboard.SubscribeTo(this, "CollisionStartWith" + GetName());
}

void Player2::Update(float dt){

	if (health > 0){

		if (health > 16){
			health = 16;
		}

		//update player2 position according to its velocity
		position += velocity;
		//move
		//set vectors to zero each frame
		direction.X = 0;
		direction.Y = 0;
		//handle movement inputs (up: y = 1, down: y = -1, left: x = -1, right: x = 1)
		if (theInput.IsKeyDown(GLFW_KEY_UP)){
			direction.Y += 1;
		}
		if (theInput.IsKeyDown(GLFW_KEY_DOWN)){
			direction.Y -= 1;
		}
		if (theInput.IsKeyDown(GLFW_KEY_LEFT)){
			direction.X -= 1;
		}
		if (theInput.IsKeyDown(GLFW_KEY_RIGHT)){
			direction.X += 1;
		}
		//fill health (for testing)
		/*if (theInput.IsKeyDown('h')){
		health = 16;
		healthBar->addHealth(16);
		}*/

		//if vector is not (0,0) then normalize it (normalizing a zero vector produces (1,0) for some reason)
		if (direction != Vector2::Zero){
			direction.Normalize();
		}
		//this->ApplyLinearImpulse(-velocity, Vector2::Zero);
		//multiply normalized vector components by the player2s maximum speed (variable needed) currently 10
		velocity.X = direction.X * 10.0f;
		velocity.Y = direction.Y * 10.0f;

		GetBody()->SetLinearVelocity(b2Vec2(velocity.X, velocity.Y));

		//if space is pressed and shooting is off cooldown then shoot
		if (theInput.IsKeyDown(GLFW_KEY_KP_0) && framesSinceLastShot >= shotCooldownFrames){
			framesSinceLastShot = 1;
			if (powerLevel == 1) { //Checks what the power of the shot is
				shotCooldownFrames = 15;
				Shoot(this->GetPosition().X, this->GetPosition().Y + 0.0f, velocity);
			}
			else if (powerLevel == 2)
			{
				shotCooldownFrames = 8;
				BeamShoot(this->GetPosition().X, this->GetPosition().Y + 0.0f, velocity);
			}
			else if (powerLevel == 3)
			{
				shotCooldownFrames = 20;
				TripleShoot(this->GetPosition().X, this->GetPosition().Y + 0.0f, velocity);
			}
		}
		else{
			framesSinceLastShot++;
		}
		//stop moving if the player2 reaches any of the edges of the screen
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
	if (health <= 0 && !dying){
		dying = true;
		theSound.PlaySound(explode, 1.0f, false, 0);
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

void Player2::ReceiveMessage(Message *message)
{
	if (message->GetMessageName() == "CollisionStartWith" + GetName())
	{
		PhysicsActor* collider = (PhysicsActor*)message->GetSender();
		if (!collider->IsDestroyed()){
			if (!isHit && !collider->IsTagged("Friendly") && !collider->IsTagged("Stage"))
			{
				health -= 1;
				if (powerLevel != 1){
					theSound.PlaySound(downgradeSound, 1.0f, false, 0);
				}
				else{
					theSound.PlaySound(hitSound, 1.0f, false, 0);
				}

				powerLevel = 1;
				healthBar->removeHealth(1);
				isHit = true;
			}
		}
		if (collider->GetName().find("TripleShot") != std::string::npos){
			powerLevel = 3;
			theSound.PlaySound(upgradeSound, 1.0f, false, 0);
		}
		else if (collider->GetName().find("BeamShot") != std::string::npos){
			powerLevel = 2;
			theSound.PlaySound(upgradeSound, 1.0f, false, 0);
		}
		else if (collider->GetName().find("Health") != std::string::npos){
			health += 4;
			if (health > 16){
				health = 16;
			}
			healthBar->addHealth(4);
			theSound.PlaySound(healthSound, 1.0f, false, 0);
		}
	}
}

void Player2::Shoot(float X, float Y, Vector2 shooterVel){
	new Shot(X, Y, 0.0, shooterVel, 0, 0, 1);
	theSound.PlaySound(shootSound, 0.5f, false, 0);
}

void Player2::BeamShoot(float X, float Y, Vector2 shooterVel){
	new Shot(X, Y, 0.0, shooterVel, 0, 1, 0);
	theSound.PlaySound(shootSound, 0.5f, false, 0);
}

void Player2::TripleShoot(float X, float Y, Vector2 shooterVel) // the angle can't be an actual angle. It's the x velocity
{
	theSound.PlaySound(shootSound, 0.5f, false, 0);
	new Shot(X, Y, 0.0, shooterVel, 0.8, 0, 1);
	new Shot(X, Y, -.05, shooterVel, 0.8, 0, 1);
	new Shot(X, Y, .05, shooterVel, 0.8, 0, 1);
}

void Player2::animateHit(){
	if (isHit){
		if (currentHitFrame <= 4){
			this->SetSpriteFrame(1);
			currentHitFrame++;
		}
		else if (currentHitFrame <= 9){
			this->SetSpriteFrame(0);
			currentHitFrame++;
		}
		else if (currentHitFrame <= 14){
			this->SetSpriteFrame(1);
			currentHitFrame++;
		}
		else if (currentHitFrame <= 19){
			this->SetSpriteFrame(0);
			currentHitFrame++;
		}
		else if (currentHitFrame <= 24){
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