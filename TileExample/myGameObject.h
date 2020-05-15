#pragma once
#include "kage2dutil/gameobject.h"

class myGameObject : public kage::GameObject
{
public:
	myGameObject();
	~myGameObject();

	//void render();
	void update(float deltaT);
	void onCollision(GameObject *obj);
};
