#include "example.h"

Example::Example(): App()
{
}

Example::~Example()
{
}

Example &Example::inst()
{
	static Example s_instance;
	return s_instance;
}

bool Example::start()
{
	//m_backgroundSprite = kage::TextureManager::getSprite("data/sky.jpg");
	//sf::Vector2u resolution = m_backgroundSprite->getTexture()->getSize();
	//m_backgroundSprite->setScale(float(m_window.getSize().x) / resolution.x, float(m_window.getSize().y) / resolution.y);
	tileMap.SetMapSize(16, 16); // Will set the map size
	kage::World::scale(32);

	// Note: setting some graphics for the two layers. First clear the layers to bricks (background) and empty (forground).
	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			tileMap.SetTile(x, y, 0, 1);
			tileMap.SetTile(x, y, 1, 0);
		}
	}
	// Note: next, draw a floor at height 12.
	for (int x = 0; x < 16; ++x)
	{
		tileMap.SetTile(x, 12, 1, 2);
	}
	// Note: finally, add a chest at 4,11.
	tileMap.SetTile(4, 11, 1, 4);

	livesText.setFont(m_font);
	livesText.setCharacterSize(40);
	livesText.setPosition(100, -10);
	livesText.setString("Lives: ");

	return true;
}

void Example::update(float deltaT)
{
	if (curState == e_Editor)
	{
		UpdateEditor(deltaT);
	}

	if (curState == e_Game)
	{
		UpdateGame(deltaT);
	}
}

void Example::render()
{
	//m_window.draw(*m_backgroundSprite);
	tileMap.render();

	// The next line draws the physics debug info. This should be removed in a final release.
	//kage::Physics::debugDraw(&m_window, 64);
}

void Example::cleanup()
{
	delete m_backgroundSprite;
}

void Example::UpdateLives()
{
	std::stringstream ss;
	ss << "Lives: " << lives; // doing this is another way of sending information out to a variable.
	livesText.setString(ss.str());
	livesText.setColor(sf::Color(255, 255, 255)); // % in this line refers to modulous, meaning it'll choose a range between etc. (Or more or so, a remainder after a divide, meaning if it goes over a value it'll wrap around, 257 would equal 0)
}

void Example::SetGameState(GameStates newState)
{
	curState = newState;
	if (curState == e_Editor)
	{
		EnterStateEditor();
	}
	if (curState == e_Game)
	{
		EnterStateGame();
	}
}

void Example::EnterStateEditor()
{
	kage::World::clear();
	kage::Physics::clearDefaultStaticFixtures();
	kage::World::setView(m_window);
}

void Example::EnterStateGame()
{
	lives = 3;
	for (size_t x = 0; x < tileMap.GetMapWidth(); x++)
	{
		for (size_t y = 0; y < tileMap.GetMapHeight(); y++)
		{
			int tile = tileMap.GetTile(x, y, 1);
			TileDefinition local = tileMap.GetTileDefinitions()[tile];
			if (tile == TileMap::TileList::e_44_SpawnPlayer)
			{
				Player* player = kage::World::build<Player>();
				player->position(x + 0.5, y + 0.5);
			}
			//else if (tile == TileMap::TileList::e_45_SpawnMonster)
			//{
			//	Enemy* enemy = kage::World::build<Enemy>();
			//	Enemy->position(x + 0.5, y + 0.5);
			//}
			else if (local.blocking)
			{
				kage::Physics::BoxBuilder().pos(kf::Vector2(x + 0.5, y + 0.5)).userData(tile).size(1, 1).build(kage::Physics::getDefaultStatic());
			}
		}
	}
}

void Example::UpdateEditor(float deltaT)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_running = false;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !ImGui::GetIO().WantCaptureMouse && m_window.hasFocus())
	{
		kf::Vector2i mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
		mousePos /= kf::Vector2i(tileMap.GetTileWidth(), tileMap.GetTileHeight()); // Mousepos will now be integer numbers rather than pixels.
		tileMap.SetTile(mousePos.x, mousePos.y, curLayer, curTile);
	}

	ImGui::Begin("Kage2D");
	if (ImGui::Button("Load") || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::O))
	{
		// Open Windows File Opening
		char mapName[MAX_PATH];
		OPENFILENAME ofn;
		ZeroMemory(&mapName, sizeof(mapName));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_window.getSystemHandle();
		ofn.lpstrFilter = "CSV Files\0*.csv\0Any File\0*.*\0";
		ofn.lpstrFile = mapName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Select folder for export";
		ofn.Flags = OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn))
		{
			tileMap.Load(mapName);
		}
	}
	if (ImGui::Button("Save") || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		// Open Windows File Saving
		char mapName[MAX_PATH];
		OPENFILENAME ofn;
		ZeroMemory(&mapName, sizeof(mapName));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_window.getSystemHandle();
		ofn.lpstrFilter = "CSV Files\0*.csv\0Any File\0*.*\0";
		ofn.lpstrFile = mapName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Select folder for export";
		ofn.Flags = OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn))
		{
			string localName = mapName;
			if (localName.find(".csv") == string::npos) // Doing this will forcibly change the filetype to CSV if not specified.
			{
				localName += ".csv";
			}
			tileMap.Save(localName.c_str());
		}
	}
	if (ImGui::Button("Play Game"))
	{
		SetGameState(e_Game);
	}
	if (ImGui::Button("Exit"))
	{
		m_running = false;
	}
	if (ImGui::InputInt("Layer", &curLayer))
	{
		curLayer = kf::clamp(curLayer, 0, 1);
	}
	for (size_t i = 0; i < tileMap.GetTileDefinitions().size(); i++)
	{
		TileDefinition local = tileMap.GetTileDefinitions()[i];
		tileMap.GetAtlas().selectTile(local.tileX, local.tileY);
		ImGui::PushID(i);
		if (ImGui::ImageButton(tileMap.GetAtlas()))
		{
			curTile = i;
		}
		if (i % 5 < 4)
		{
			ImGui::SameLine();
		}
		ImGui::PopID();
	}

	ImGui::End();
}

void Example::UpdateGame(float deltaT)
{
	if (lives > 0)
	{
		Player* player = (Player*)kage::World::findByTag("Player");
		CameraSmoothing = kf::lerp(player->velocity(), CameraSmoothing, 0.98);
		kage::World::setView(m_window, player->position() - (CameraSmoothing / 4), 0.45);
	}
	ImGui::Begin("Kage2D");
	if (ImGui::Button("Leave to Editor"))
	{
		SetGameState(e_Editor);
	}
	if (ImGui::Button("Exit"))
	{
		m_running = false;
	}
	ImGui::Text("Lives: %d", lives);
	ImGui::InputInt("Lives", &lives);
	ImGui::End();
}