#pragma once

#include "app.h"
#include "kage2dutil/physics.h"
#include "kage2dutil/atlas.h"	// Note: added so the atlas class can be put in the Example class.
#include "TileMap.h"

class Example : public App
{
public:
	Example();
	virtual ~Example();
	virtual bool start();
	virtual void update(float deltaT);
	virtual void render();
	virtual void cleanup();
	static Example &inst();

	sf::Sprite *m_backgroundSprite;
	kage::Atlas m_atlas;	// Note: this is the atlas object that will be used to render the map.
	std::vector<int> m_tiles[2]; // Note: craete an array of 2 vectors of ints. This stores the 2 layers of our map.

	TileMap tileMap;
};
