#include "stdafx.h"
#include "GameManager.h"

AerocideGameManager* AerocideGameManager::s_AerocideGameManager = NULL;

AerocideGameManager::AerocideGameManager()
{
	//subscribe to messages
	theSwitchboard.SubscribeTo(this, "ToggleDebugInfo");

	idealFPS = 60;

	text = new TextActor("Console", "Press Enter to Begin");
	text->SetPosition(0, 1);
	text->SetAlignment(TXT_Center);
	text->SetLayer(4); //text layer
	text->SetColor(0,1,0,1);
	theWorld.Add(text);

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

	player = new Player();

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
	if (!gameStarted){
		theWorld.PauseSimulation();
		if (theInput.IsKeyDown(GLFW_KEY_ENTER)){
			gameStarted = true;
			theWorld.Remove(text);
			theWorld.ResumeSimulation();
		}
	}
	else{

		if (player->IsDestroyed()){
			text = new TextActor("Console", "Game Over. Press 'Esc' to exit.");
			text->SetPosition(0, 1);
			text->SetAlignment(TXT_Center);
			text->SetLayer(4); //text layer
			text->SetColor(0, 1, 0, 1);
			theWorld.Add(text);
			theWorld.PauseSimulation();
		}

		//calcualte fps
		realFPS = 1 / (glfwGetTime() - lastFrameTime);
		while (realFPS > idealFPS){
			//delay framerate
			realFPS = 1 / (glfwGetTime() - lastFrameTime);
		}
		//update displayed fps only once per second
		/*if (glfwGetTime() - lastFPSUpdateTime >= 1){
			debugInfoFPS = realFPS;
			lastFPSUpdateTime = glfwGetTime();
			}*/
		if (realFPS <= idealFPS){
			debugInfoFPS = realFPS;
		}
		//if end of stage not reached continue scrolling
		if (stagePosition.Y > (-426.666 / 2.0f) + 11 && !player->IsDestroyed()){ //half the stage images height plus half the screens height and 1 as a buffer from overshooting the edge of the image
			//update stage position according to its velocity
			stagePosition.Y += stageVel.Y;
		}





		if (stagePosition.Y <= 185 && !asteroidSpawned){ //Spawning asteroid field
			new Asteroid(-3, 25, 0.5, -6);
			new Asteroid(4, 24, 1, -5);
			new Asteroid(-3.5, 23, 0.7, -6.5);
			new Asteroid(-1, 26, 1.5, -6.2);
			new Asteroid(-7, 25, 1.3, -6.4);
			new Asteroid(6, 27, -0.2, -5.4);
			new Asteroid(9, 25.4, -0.3, -5.5);
			new Asteroid(5.5, 24.6, -1.2, -7);
			new Asteroid(-3.7, 23.4, 1.7, -5.7);
			new Asteroid(-8, 26.7, 0.4, -6.3);
			new Asteroid(-9, 26.3, 0.3, -6.25);
			new Asteroid(0.3, 24.5, -0.25, -5.8);
			new Asteroid(-2.5, 23.8, 1.25, -5.9);
			new Asteroid(4.6, 23, -0.55, -5.5);
			new Asteroid(2, 25.5, -0.59, -5.5);
			new Asteroid(6.7, 25.3, -1.1, -5.6);
			new Asteroid(0.75, 24.7, -1.05, -5.3);
			new Asteroid(8.8, 24.45, -1.8, -5.2);
			new Asteroid(-5, 26.4, 2, 1.1);
			new Asteroid(3.5, 26.8, -0.6, -5.25);
			new BigAsteroid(0, 25, -8.1, -210);
			new BigAsteroid(-1.2, 26, 9.2, -240);
			new BigAsteroid(-2.0, 23.5, 12, -190);
			new BigAsteroid(-3.4, 24.2, -14.1, -220);
			new BigAsteroid(4, 24.5, -10.1, -215);
			new HugeAsteroid(0, 27, 4, -1500);
			asteroidSpawned = true;
		}
		/*
		if (stagePosition.Y <= 185 && !BigAsteroidSpawned){ //Spawning asteroid field
			new BigAsteroid(-3, 25, 0.5, -6);
			BigAsteroidSpawned = true;
		}*/

		//move stage
		stage->SetPosition(stagePosition);

		//store time completed
		lastFrameTime = glfwGetTime();
	}
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

void AerocideGameManager::SoundEnded(AngelSoundHandle sound)
{
	// Detect sounds that have ended here.
}