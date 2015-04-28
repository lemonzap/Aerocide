//////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2008-2013, Shane Liesegang
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// 
//     * Redistributions of source code must retain the above copyright 
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright 
//       notice, this list of conditions and the following disclaimer in the 
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holder nor the names of any 
//       contributors may be used to endorse or promote products derived from 
//       this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include "Shot.h"
#include "Asteroid.h"
#include "Player.h"

//AerocideGameManager is a singleton and inherits from:
//  GameManager -- so we receive update and render calls, plus collision notifications
#define theGame AerocideGameManager::GetInstance()

class AerocideGameManager : public GameManager
{
public:

	void ToggleDebugInfo();

	static AerocideGameManager &GetInstance();

	virtual void Update(float dt);

	virtual void Render();

	virtual void SoundEnded(AngelSoundHandle sound);

	virtual void ReceiveMessage(Message* message);

protected:
	AerocideGameManager();
	static AerocideGameManager *s_AerocideGameManager;

private:
	Player* player;
	Actor* stage;
	TextActor* text;
	Vector2 stagePosition;
	Vector2 stageVel;
	float idealFPS;
	float realFPS = 0;
	float lastFrameTime = 0;
	float debugInfoFPS = 0;
	float lastFPSUpdateTime = 0;
	bool debugInfo = false;
	bool asteroidSpawned = false;
	bool gameStarted = false;
};
