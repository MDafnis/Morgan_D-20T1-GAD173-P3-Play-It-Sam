#pragma once
#include <vector>
#include <TileDefinition.h>
#include <string>
#include <iostream>
#include <kage2dutil/atlas.h>
using namespace std;

class TileMap 
{
private:
	int mapWidth;
	int mapHeight;
	int tileWidth;
	int tileHeight;
	kage::Atlas atlas;
	vector<TileDefinition> tileDefinitions;
	vector<int> tiles[2];
public:
	TileMap();
	//static TileMap& inst();
	virtual void SetMapSize(int width, int height);
	virtual void SetTileSize(int width, int height);
	/*TileDefinition& GetTileDefinitions()
	{
		bool blocking = TileDefinition::blocking;
		bool visibleInGame = TileDefinition::visibleInGame;
		int tileX = TileDefinition::tileX;
		int tileY = TileDefinition::tileY;
	}*/
	virtual int GetMapWidth();
	virtual int GetMapHeight();
	virtual int GetTileWidth();
	virtual int GetTileHeight();
	virtual void Clear(int tile);
	virtual void Load(string fileName);
	virtual void Save(string fileName);
	virtual int GetTile(int x, int y, int layer);
	virtual void SetTile(int x, int y, int layer, int tile);
	virtual void render();
	virtual vector<TileDefinition>& GetTileDefinitions();
	virtual kage::Atlas& GetAtlas();
	enum TileList
	{
		e_0_EmptyTile = 0,
		e_1_BackgroundStone0,
		e_2_BlockingStone0,
		e_3_BackgroundStone1,
		e_4_BlockingStone1,
		e_44_SpawnPlayer,
		e_45_SpawnEnemy
	};
};
