/*
GROUP MEMBER #1: GABE NELSON / LAB TA: CHASE PUGLISI 
GROUP MEMBER #2: NATHAN HENRY / LAB TA: TRAVIS LANE
*/
#include "stdafx.h"

#include "GameManager.h"

int main(int argc, char* argv[])
{
	// get things going
	//  optional parameters:
	//		int windowWidth			default: 1024
	//		int windowHeight		default: 768
	// std::string windowName		default: "Angel Engine"
	//		bool antiAliasing		default: false
	//		bool fullScreen			default: false
	//		bool resizable			default: false
	theWorld.Initialize(750,768,"Aerocide",false, false, false);

	theWorld.SetupPhysics(Vector2(0.0f, 0.0f));

	//adds the default grid so you can more easily place Actors
	theWorld.Add(new GridActor(), -1);
	
	//YOUR GAME SETUP CODE HERE
	
	theWorld.SetGameManager(&theGame);
	
	// do all your setup first, because this function won't return until you're exiting
	theWorld.StartGame();
	
	// any cleanup can go here
	theWorld.Destroy();
	
	return 0;
}
