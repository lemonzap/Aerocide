#include "stdafx.h"
#include "GameManager.h"

AerocideGameManager* AerocideGameManager::s_AerocideGameManager = NULL;

AerocideGameManager::AerocideGameManager()
{
	//subscribe to messages
	theSwitchboard.SubscribeTo(this, "ToggleDebugInfo");

	//setup stage
	stage = new Actor();
	stage->SetSize(20.0f, 426.666f);
	stage->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	stage->ClearSpriteInfo();
	stage->SetSprite("Resources/Images/TestStage.png", 0, GL_CLAMP, GL_NEAREST, false);
	stage->SetLayer(0); //background layer
	theWorld.Add(stage);
	stagePosition.X = 0.0f;
	stagePosition.Y = (426.666f/2.0f) - 10; //Half the stage images height - half the screens height
	stage->SetPosition(stagePosition);
	stageVel.X = 0.0f;
	stageVel.Y = -0.1f;

	//setup player
	playerShip = new Actor();
	playerShip->SetSize(1.0f);
	playerShip->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	playerShip->ClearSpriteInfo();
	playerShip->SetSprite("Resources/Images/Ship.png", 0, GL_CLAMP, GL_NEAREST, false);
	playerShip->SetLayer(3); //player layer
	theWorld.Add(playerShip);
	playerPosition.X = 0.0f;
	playerPosition.Y = 0.0f;
	playerShip->SetPosition(playerPosition);
	playerVel.X = 0.00f;
	playerVel.Y = 0.00f;

	//initialize first frame time in the past so that the first frame will run
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

//called whenever a message that has been subscribed to is generated
void AerocideGameManager::ReceiveMessage(Message* message)
{
	//if the message received was "ToggleDebugInfo" then toggle debug info
	if (message->GetMessageName() == "ToggleDebugInfo"){
		ToggleDebugInfo();
	}
}

void AerocideGameManager::Update(float dt)
{
	//calcualte fps
	realFPS = 1 / (glfwGetTime() - lastFrameTime);
		//update displayed fps only once per second
		if (glfwGetTime() - lastFPSUpdateTime >= 1){
			debugInfoFPS = realFPS;
			lastFPSUpdateTime = glfwGetTime();
		}

		//if end of stage not reached continue scrolling
		if (stagePosition.Y > (-426.666 / 2.0f) + 11){ //half the stage images height plus half the screens height and 1 as a buffer from overshooting the edge of the image
			//update stage position according to its velocity
			stagePosition.Y += stageVel.Y;
		}
		//set vectors to zero each frame
		playerVel.X = 0.0f;
		playerVel.Y = 0.0f;
		playerDirection.X = 0;
		playerDirection.Y = 0;

		//handle movement inputs (up: y = 1, down: y = -1, left: x = -1, right: x = 1)
		if (theInput.IsKeyDown('w')){
			playerDirection.Y += 1;
		}
		if (theInput.IsKeyDown('s')){
			playerDirection.Y -= 1;
		}
		if (theInput.IsKeyDown('a')){
			playerDirection.X -= 1;
		}
		if (theInput.IsKeyDown('d')){
			playerDirection.X += 1;
		}
		//if vector is not (0,0) then normalize it (normalizing a zero vector produces (1,0) for some reason)
		if (playerDirection != Vector2::Zero){
			playerDirection.Normalize();
		}
		//multiply normalized vector components by the players maximum speed (variable needed)
		playerVel.X += playerDirection.X * 0.15f;
		playerVel.Y += playerDirection.Y * 0.15f;

		//if space is pressed and shooting is off cooldown then shoot
		if (theInput.IsKeyDown(' ') && framesSinceLastShot >= shotCooldownFrames){
			framesSinceLastShot = 1;
			Shoot(playerPosition.X, playerPosition.Y + 0.0f, playerVel);
		}else{
			framesSinceLastShot++;
		}
		//update player position according to its velocity
		playerPosition += playerVel;
		//stop moving if the player reaches any of the edges of the screen
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

		//move stage
		stage->SetPosition(stagePosition);
		//move player
		playerShip->SetPosition(playerPosition);

		//store time completed
		lastFrameTime = glfwGetTime();
}

void AerocideGameManager::Render()
{
	//if debug info is requested then display the fps (more information will be included later. toggle debug info with f3)
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