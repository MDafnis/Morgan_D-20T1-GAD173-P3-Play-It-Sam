#pragma once
#include <vector>
#include <TileDefinition.h>
using namespace std;
using namespace kage;

class TileMap 
{
private:
	int mapWidth,mapHeight,tileWidth,tileHeight;
	vector<int> tiles[2];
	Atlas atlas;
	vector<TileDefinition> tileDefinitions;
public:
	void SetMapSize(int width, int height) {}
	void SetTileSize(int width, int height) {}
	int GetMapWidth() {}
	int GetMapHeight() {}
	int GetTileWidth() {}
	int GetTileHeight() {}
	TileDefinition& GetTileDefinitions() {}
	void render() {}
	void Clear(int tile) {}
	void Load(string filename) {}
	void Save(string filename) {}
	int GetTile(int x, int y, int layer) {}
	void SetTile(int x, int y, int layer, int tile) {}
};
