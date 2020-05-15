#pragma once

#include "app.h"
#include "kage2dutil/physics.h"
#include "kage2dutil/atlas.h"	// Note: added so the atlas class can be put in the Example class.
#include "TileMap.h"
#include "Player.h"

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
	// MAP Handling
	int mapSize;
	sf::Sprite *m_backgroundSprite;
	kage::Atlas m_atlas;	// Note: this is the atlas object that will be used to render the map.
	TileMap tileMap;
	int curTile, curLayer;
	// Game Handling
	void UpdateEditor(float deltaT);
	void UpdateGame(float deltaT);
	int lives = 3;
	sf::Text livesText;
	void UpdateLives();
	void ToggleEditor();
	enum GameStates
	{
		e_Editor,
		//e_MainMenu,
		//e_GameBegin,
		e_Game//,
		//e_GameEnd
	};
	GameStates curState = GameStates::e_Editor;
	void SetGameState(GameStates newState);
	void EnterStateEditor();
	//void EnterStateMainMenu();
	//void EnterStateGameBegin();
	void EnterStateGame();
	//void EnterStateGameEnd();
	// Character Illusions
	kf::Vector2 CameraSmoothing;
};
