#include "TileMap.h"
#include "example.h"

TileMap::TileMap()
{
	atlas.create("data/CookieCutterAtlas.png", 32, 32);
	SetTileSize(32, 32);

	TileDefinition td;
	// 0_EmptyTile or Template tile
	td.blocking = false;
	td.visibleInGame = false;
	td.tileX = 0; // column
	td.tileY = 0; // row
	tileDefinitions.push_back(td);
	// 1_BackgroundStone0
	td.blocking = false;
	td.visibleInGame = true;
	td.tileX = 1; // column
	td.tileY = 0; // row
	tileDefinitions.push_back(td);
	// 2_BlockingStone0
	td.blocking = true;
	td.visibleInGame = true;
	td.tileX = 2; // column
	td.tileY = 0; // row
	tileDefinitions.push_back(td);
	// 3_BackgroundStone1
	td.blocking = false;
	td.visibleInGame = true;
	td.tileX = 3; // column
	td.tileY = 0; // row
	tileDefinitions.push_back(td);
	// 4_BlockingStone1
	td.blocking = true;
	td.visibleInGame = true;
	td.tileX = 4; // column
	td.tileY = 0; // row
	tileDefinitions.push_back(td);
	td.blocking = false;
	td.visibleInGame = false;
	td.tileX = 11; // column
	td.tileY = 2; // row
	tileDefinitions.push_back(td);
}

vector<TileDefinition>& TileMap::GetTileDefinitions()
{
	return tileDefinitions;
}

kage::Atlas& TileMap::GetAtlas()
{
	return atlas;
}

void TileMap::SetMapSize(int width, int height)
{
	mapWidth = width;
	mapHeight = height;
	tiles[0].resize(width * height);
	tiles[1].resize(width * height);
}
void TileMap::SetTileSize(int width, int height)
{
	tileWidth = width;
	tileHeight = height;
}
int TileMap::GetMapWidth()
{
	return mapWidth;
}
int TileMap::GetMapHeight()
{
	return mapHeight;
}
int TileMap::GetTileWidth()
{
	return tileWidth;
}
int TileMap::GetTileHeight()
{
	return tileHeight;
}
void TileMap::render()
{
	sf::RenderWindow& window = Example::inst().getWindow();
	// Note: render layer 0 (background)
	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			TileDefinition local = tileDefinitions[tiles[0][x + y * mapWidth]]; // tiles[0][x + y * mapWidth] similar to GetTile

			if (local.visibleInGame || Example::inst().curState == Example::GameStates::e_Editor) // Stops the game state from being able to view invisible 
			{																				      // tiles.
				atlas.draw(window, x * 32, y * 32, local.tileX, local.tileY);
			}
		}
	}
	// Note: render layer 1 (foreground)
	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			TileDefinition local = tileDefinitions[tiles[1][x + y * mapWidth]]; // tiles[0][x + y * mapWidth] similar to GetTile

			if (local.visibleInGame || Example::inst().curState == Example::GameStates::e_Editor) // Stops the game state from being able to view invisible
			{																				      // tiles.
				atlas.draw(window, x * 32, y * 32, local.tileX, local.tileY);
			}
		}
	}
}
/*TileDefinition& GetTileDefinitions()
{

}*/
void TileMap::Clear(int tile)
{
	for (size_t l = 0; l < 2; l++)
	{
		for (size_t y = 0; y < mapHeight; y++)
		{
			for (size_t x = 0; x < mapWidth; x++)
			{
				SetTile(x, y, l, tile);
			}
		}
	}
}
void TileMap::Load(string fileName)
{
	fstream mapFile;
	mapFile.open(fileName, ios::in);// IOS In reads a file.
	int width, height;
	mapFile >> width;
	mapFile.ignore(1, ',');
	mapFile >> height;
	mapFile.ignore(1, ',');
	SetMapSize(width, height);
	for (size_t l = 0; l < 2; l++)
	{
		for (size_t y = 0; y < mapHeight; y++)
		{
			for (size_t x = 0; x < mapWidth; x++)
			{
				int tile;
				mapFile >> tile;
				mapFile.ignore(1, ',');
				SetTile(x, y, l, tile);
				if (mapFile.eof())
				{
					y = mapHeight;
					cout << "There is a problem at tile " << (x + y * mapWidth);
					break;
				}
			}
		}
	}
}
void TileMap::Save(string fileName)
{
	fstream mapFile;
	mapFile.open(fileName, ios::out);// IOS Out writes to a file.
	mapFile << mapWidth << "," << mapHeight << endl;
	for (size_t l = 0; l < 2; l++)
	{
		for (size_t y = 0; y < mapHeight; y++)
		{
			for (size_t x = 0; x < mapWidth; x++)
			{
				mapFile << GetTile(x, y, l);
				if (x != mapWidth - 1)
				{
					mapFile << ",";
				}
				mapFile << endl;
			}
		}
	}
}
int TileMap::GetTile(int x, int y, int layer)
{
	if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight && layer >= 0 && layer < 2)
	{
		return tiles[layer][x + y * mapWidth];
	}
	return 0;
}
void TileMap::SetTile(int x, int y, int layer, int tile)
{
	if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight && layer >= 0 && layer < 2)
	{
		tiles[layer][x + y * mapWidth] = tile;
	}
}
