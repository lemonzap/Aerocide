#include "stdafx.h"
#include "Player2.h"


Player2::Player2(){
	health = 16;
	score = 0;
	shotCooldownFrames = 15;
	//setup player2
	healthBar = new HealthBar(health, 0, 7.8);
	this->SetSize(1.0f);
	this->SetPosition(5, 0);
	this->SetColor(0.5, 1, 0.5, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/ShipLeftTilt.png", 2, GL_CLAMP, GL_NEAREST, false);
	this->SetSprite("Resources/Images/ShipRightTilt.png", 3, GL_CLAMP, GL_NEAREST, false);
	this->SetSprite("Resources/Images/ShipHit.png", 1, GL_CLAMP, GL_NEAREST, false);
	this->SetSprite("Resources/Images/Ship.png", 0, GL_CLAMP, GL_NEAREST, false);
	shootSound = theSound.LoadSample("Resources/Sounds/Shoot2.wav", false);
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
		this->SetSpriteFrame(0);
		if (theInput.IsKeyDown(GLFW_KEY_UP)){
			direction.Y += 1;
		}
		if (theInput.IsKeyDown(GLFW_KEY_DOWN)){
			direction.Y -= 1;
		}
		if (theInput.IsKeyDown(GLFW_KEY_LEFT)){
			direction.X -= 1;
			this->SetSpriteFrame(2);
		}
		if (theInput.IsKeyDown(GLFW_KEY_RIGHT)){
			direction.X += 1;
			this->SetSpriteFrame(3);
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
				Shoot(this->GetPosition().X, this->GetPosition().Y + 0.0f, velocity, false);
			}
			else if (powerLevel == 2) //faster shot
			{
				shotCooldownFrames = 8;
				BeamShoot(this->GetPosition().X, this->GetPosition().Y + 0.0f, velocity, false);
			}
			else if (powerLevel == 3) //triple shot
			{
				shotCooldownFrames = 15; //I changed this from 20 to 15 for balancing reasons. the beam is more useful when the triple is restricted to 20
				TripleShoot(this->GetPosition().X, this->GetPosition().Y + 0.0f, velocity, false);
			}
			else if (powerLevel == 4){
				shotCooldownFrames = 8;
				TripleShoot(this->GetPosition().X, this->GetPosition().Y + 0.0f, velocity, false);
			}
			else if (powerLevel == 5){
				shotCooldownFrames = 15;
				Shoot(this->GetPosition().X, this->GetPosition().Y + 0.0f, velocity, true);
			}
			else if (powerLevel == 6){
				shotCooldownFrames = 15;
				BeamShoot(this->GetPosition().X, this->GetPosition().Y + 0.0f, velocity, true);
			}
			else if (powerLevel == 7){
				shotCooldownFrames = 8;
				TripleShoot(this->GetPosition().X, this->GetPosition().Y + 0.0f, velocity, true);
			}
			else if (powerLevel == 8){
				shotCooldownFrames = 8;
				TripleShoot(this->GetPosition().X, this->GetPosition().Y + 0.0f, velocity, true);
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

		switch (powerLevel){
		case 1:
			score += 0;
			break;
		case 2:
			currentUpgradeFrame++;
			if (currentUpgradeFrame >= 30){
				currentUpgradeFrame = 0;
				score += 1;
			}
			break;
		case 3:
			currentUpgradeFrame++;
			if (currentUpgradeFrame >= 30){
				currentUpgradeFrame = 0;
				score += 1;
			}
			break;
		case 4:
			currentUpgradeFrame++;
			if (currentUpgradeFrame >= 30){
				currentUpgradeFrame = 0;
				score += 2;
			}
			break;
		case 5:
			currentUpgradeFrame++;
			if (currentUpgradeFrame >= 30){
				currentUpgradeFrame = 0;
				score += 1;
			}
			break;
		case 6:
			currentUpgradeFrame++;
			if (currentUpgradeFrame >= 30){
				currentUpgradeFrame = 0;
				score += 2;
			}
			break;
		case 7:
			currentUpgradeFrame++;
			if (currentUpgradeFrame >= 30){
				currentUpgradeFrame = 0;
				score += 2;
			}
			break;
		case 8:
			currentUpgradeFrame++;
			if (currentUpgradeFrame >= 30){
				currentUpgradeFrame = 0;
				score += 3;
			}
			break;
		}

		animateHit();
	}
	if (health <= 0 && !dying){ //player2 blows up if they die
		dying = true;
		theSound.PlaySound(explode, 1.0f, false, 0);
		this->LoadSpriteFrames("Resources/Images/Explosion_001.png", GL_CLAMP, GL_NEAREST);
		this->PlaySpriteAnimation(0.1, SAT_Loop, 0, 6, "explode");
	}
	if (dying){ //the death animation
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

void Player2::ReceiveMessage(Message *message) //player2 colliding with something
{
	try{
		if (message->GetMessageName() == "CollisionStartWith" + GetName())
		{
			PhysicsActor* collider = (PhysicsActor*)message->GetSender();
			if (!collider->IsDestroyed()){
				if (!isHit && !collider->IsTagged("Friendly") && !collider->IsTagged("Stage") && !collider->IsTagged("Ship"))
				{ //enemy or enemy shot
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
				score += 5;
				if (powerLevel == 2 || powerLevel == 4){
					powerLevel = 4;
				}
				else if (powerLevel == 5 || powerLevel == 7){
					powerLevel = 7;
				}
				else if (powerLevel == 6 || powerLevel == 8){
					powerLevel = 8;
				}
				else{
					powerLevel = 3;//upgrades the shot to triple and plays sound
				}
				theSound.PlaySound(upgradeSound, 1.0f, false, 0);
			}
			else if (collider->GetName().find("BeamShot") != std::string::npos){
				score += 5;
				if (powerLevel == 3 || powerLevel == 4){
					powerLevel = 4;
				}
				else if (powerLevel == 5 || powerLevel == 6){
					powerLevel = 6;
				}
				else if (powerLevel == 7 || powerLevel == 8){
					powerLevel = 8;
				}
				else{
					powerLevel = 2;//upgrades the shot to increase firerate and play sound
				}
				theSound.PlaySound(upgradeSound, 1.0f, false, 0);
			}
			else if (collider->GetName().find("PierceShot") != std::string::npos){
				score += 5;
				if (powerLevel == 3 || powerLevel == 7){
					powerLevel = 7;
				}
				else if (powerLevel == 2 || powerLevel == 6){
					powerLevel = 6;
				}
				else if (powerLevel == 3 || powerLevel == 8){
					powerLevel = 8;
				}
				else{
					powerLevel = 5;//upgrades the shot to pierce and play sound
				}
				theSound.PlaySound(upgradeSound, 1.0f, false, 0);
			}
			else if (collider->GetName().find("Health") != std::string::npos){
				score += 5;
				health += 4; //adds 4 health to the player2 and plays the sound
				if (health > 16){
					health = 16;
				}
				healthBar->addHealth(4);
				theSound.PlaySound(healthSound, 1.0f, false, 0);
			}
			else if (collider->GetName().find("BigAsteroid") != std::string::npos){
				health -= 1; //lose an extra 1 health if big asteroid
				healthBar->removeHealth(1);
			}
			else if (collider->GetName().find("HugeAsteroid") != std::string::npos){
				health -= 2; //lose an extra 2 health if huge asteroid
				healthBar->removeHealth(2);
			}
		}
	}
	catch (int e){
		std::cout << "Player2 Error" << std::endl;
	}
}

void Player2::Shoot(float X, float Y, Vector2 shooterVel, bool pierce){ //makes a shot function class when shooting
	if (!pierce){
		new Shot(X, Y, 0.0, shooterVel, 0, 0, 1, false, pierce);
	}
	else{
		new Shot(X, Y, 0.0, shooterVel, 1, 1, 0, false, pierce);
	}
	theSound.PlaySound(shootSound, 0.5f, false, 0);
}

void Player2::BeamShoot(float X, float Y, Vector2 shooterVel, bool pierce){ //makes the faster shot with a different color
	if (!pierce){
		new Shot(X, Y, 0.0, shooterVel, 0, 1, 0, false, pierce);
	}
	else{
		new Shot(X, Y, 0.0, shooterVel, 1, 1, 0, false, pierce);
	}
	theSound.PlaySound(shootSound, 0.5f, false, 0);
}

void Player2::TripleShoot(float X, float Y, Vector2 shooterVel, bool pierce) // the angle can't be an actual angle. It's the x velocity
{
	theSound.PlaySound(shootSound, 0.5f, false, 0);
	if (powerLevel == 3){
		new Shot(X, Y, 0.0, shooterVel, 0.8, 0, 1, false, pierce);
		new Shot(X, Y, -0.05, shooterVel, 0.8, 0, 1, false, pierce);
		new Shot(X, Y, 0.05, shooterVel, 0.8, 0, 1, false, pierce);
	}
	else if (powerLevel == 4){
		new Shot(X, Y, 0.0, shooterVel, 0, 1, 0, false, pierce);
		new Shot(X, Y, -0.05, shooterVel, 0, 1, 0, false, pierce);
		new Shot(X, Y, 0.05, shooterVel, 0, 1, 0, false, pierce);
	}
	else if (powerLevel == 8){
		new Shot(X, Y, 0.0, shooterVel, 0, 0, 0, true, pierce);
		new Shot(X, Y, -0.05, shooterVel, 0, 0, 0, true, pierce);
		new Shot(X, Y, 0.05, shooterVel, 0, 0, 0, true, pierce);
	}
	else if (pierce){
		new Shot(X, Y, 0.0, shooterVel, 1, 1, 0, false, pierce);
		new Shot(X, Y, -0.05, shooterVel, 1, 1, 0, false, pierce);
		new Shot(X, Y, 0.05, shooterVel, 1, 1, 0, false, pierce);
	}
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