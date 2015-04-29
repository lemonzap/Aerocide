#include "stdafx.h"
#include "healthBar.h"


HealthBar::HealthBar(int initialHealth, float x, float y){
	health = initialHealth;
	this->SetSize(6, 1.5f);
	this->SetPosition(Vector2(x, y));
	this->SetColor(1, 1, 1, 1); //(white and opaque so the texture comes through fully)
	this->ClearSpriteInfo();
	this->SetSprite("Resources/Images/HealthBar.png", 0, GL_CLAMP, GL_NEAREST, false);
	this->SetLayer(4); //Health Bar layer
	theWorld.Add(this);
	/*healthPoints[0] = new HealthPoint(this->GetPosition().X + 1.32, this->GetPosition().Y);
	healthPoints[1] = new HealthPoint(this->GetPosition().X + 1.15, this->GetPosition().Y);
	healthPoints[1] = new HealthPoint(this->GetPosition().X + 0.98, this->GetPosition().Y);*/
	for (int i = 0; i < initialHealth; i++){
		healthPoints[i] = new HealthPoint(this->GetPosition().X + (1.32f - (0.188f * i)), this->GetPosition().Y);
	}
}

void HealthBar::removeHealth(int amount){
	int prevHealth = health;
	while (prevHealth - amount < health){
		this->healthPoints[health-1]->SetLayer(3); //below health bar
		health--;
	}
}

void HealthBar::addHealth(int amount){
	int prevHealth = health;
	while (prevHealth + amount > health && health < 16){
		this->healthPoints[health]->SetLayer(5); //below health bar
		health++;
	}
}