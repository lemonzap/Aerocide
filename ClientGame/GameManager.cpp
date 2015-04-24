#include "stdafx.h"
#include "GameManager.h"

AerocideGameManager* AerocideGameManager::s_AerocideGameManager = NULL;

AerocideGameManager::AerocideGameManager()
{
	//subscribe to messages
	theSwitchboard.SubscribeTo(this, "ToggleDebugInfo");

	stage = new Actor();
	stage->SetSize(20.0f, 426.666f);
	stage->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	stage->ClearSpriteInfo();
	stage->SetSprite("Resources/Images/TestStage.png", 0, GL_CLAMP, GL_NEAREST, false);
	stage->SetLayer(0);
	theWorld.Add(stage);
	stagePosition.X = 0.0f;
	stagePosition.Y = (426.666f/2.0f) - 10; //Half the stage images height - half the screens height
	stage->SetPosition(stagePosition);
	stageVel.X = 0.0f;
	stageVel.Y = -0.1f;

	playerShip = new Actor();
	playerShip->SetSize(1.0f);
	playerShip->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	playerShip->ClearSpriteInfo();
	playerShip->SetSprite("Resources/Images/Ship.png", 0, GL_CLAMP, GL_NEAREST, false);
	playerShip->SetLayer(3);
	theWorld.Add(playerShip);
	playerPosition.X = 0.0f;
	playerPosition.Y = 0.0f;
	playerShip->SetPosition(playerPosition);
	playerVel.X = 0.00f;
	playerVel.Y = 0.00f;

	lastFrameTime = glfwGetTime() - 1;

	// We must set the sound callback method.  Although, I'm wondering if we should 
	// allow them to not specify it if they don't need the functionality.
	theSound.SetSoundCallback(this, &GameManager::SoundEnded);
}

AerocideGameManager& AerocideGameManager::GetInstance()
{
	if (s_AerocideGameManager == NULL)
	{
		s_AerocideGameManager = new AerocideGameManager();
	}
	return *s_AerocideGameManager;
}

void AerocideGameManager::ReceiveMessage(Message* message)
{
	if (message->GetMessageName() == "ToggleDebugInfo"){
		ToggleDebugInfo();
	}
}

void AerocideGameManager::Update(float dt)
{
	realFPS = 1 / (glfwGetTime() - lastFrameTime);
		if (glfwGetTime() - lastFPSUpdateTime >= 1){
			debugInfoFPS = realFPS;
			lastFPSUpdateTime = glfwGetTime();
		}


		if (stagePosition.Y > (-426.666 / 2.0f) + 11){ //half the stage images height plus half the screens height and 1 as a buffer from overshooting the edge of the image
			stagePosition.Y += stageVel.Y;
		}
		playerVel.X = 0.0f;
		playerVel.Y = 0.0f;
		if (theInput.IsKeyDown('w')){
			playerVel.Y += 0.15f;
		}
		if (theInput.IsKeyDown('s')){
			playerVel.Y -= 0.15f;
		}
		if (theInput.IsKeyDown('a')){
			playerVel.X -= 0.15f;
		}
		if (theInput.IsKeyDown('d')){
			playerVel.X += 0.15f;
		}
		if (theInput.IsKeyDown(' ') && framesSinceLastShot >= shotCooldownFrames){
			framesSinceLastShot = 1;
			Shoot(playerPosition.X, playerPosition.Y + 0.0f, playerVel);
		}
		else{
			framesSinceLastShot++;
		}
		playerPosition += playerVel;
		if (playerPosition.X < -9.5){
			playerPosition.X = -9.5;
		}
		else if (playerPosition.X > 9.5){
			playerPosition.X = 9.5;
		}
		if (playerPosition.Y < -9.5){
			playerPosition.Y = -9.5;
		}
		else if (playerPosition.Y > 9.5){
			playerPosition.Y = 9.5;
		}
		stage->SetPosition(stagePosition);
		playerShip->SetPosition(playerPosition);

		lastFrameTime = glfwGetTime();
}

void AerocideGameManager::Render()
{
	if (debugInfo){
		glColor3f(1.0f, 0.0f, 0.0f);
		DrawGameText("FPS: " + std::to_string(debugInfoFPS), "ConsoleSmall", 4, 18);
	}
}

void AerocideGameManager::ToggleDebugInfo(){
	debugInfo = !debugInfo;
}

void AerocideGameManager::Shoot(float X, float Y, Vector2 shooterVel){
	new Shot(X,Y,shooterVel);
}

void AerocideGameManager::SoundEnded(AngelSoundHandle sound)
{
	// Detect sounds that have ended here.
}