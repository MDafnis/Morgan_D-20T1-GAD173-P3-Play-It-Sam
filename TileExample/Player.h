#pragma once
#include "kage2dutil/gameobject.h"
#include "example.h"
#include <windows.h>

class Player : public kage::GameObject
{
public:
	Player();
	bool leapingFromWall;
	bool lastUp = false;
	float timer = 0.15;
	bool deathTick;
	float deathTimer;

	//void render();
	void update(float deltaT);
	void onCollision(GameObject* obj);
	void onCollision(b2Fixture* fix);
};
