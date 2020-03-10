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
	m_backgroundSprite = kage::TextureManager::getSprite("data/sky.jpg");
	sf::Vector2u resolution = m_backgroundSprite->getTexture()->getSize();
	m_backgroundSprite->setScale(float(m_window.getSize().x) / resolution.x, float(m_window.getSize().y) / resolution.y);
	m_atlas.create("data/rpg_tiles.png", 32, 32);  // Note: This loads the atlas texture and sets it to use 32x32 pixel tiles.
	m_tiles[0].resize(16 * 16); // Note: resize layer 0 (background) to be 16x16 tiles.
	m_tiles[1].resize(16 * 16); // Note: resize layer 1 (background) to be 16x16 tiles.

	// Note: setting some graphics for the two layers. First clear the layers to bricks (background) and empty (forground).
	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			m_tiles[0][x + y * 16] = 265;
			m_tiles[1][x + y * 16] = 0;
		}
	}
	// Note: next, draw a floor at height 12.
	for (int x = 0; x < 16; ++x)
	{
		m_tiles[1][x + 12 * 16] = 266;
	}
	// Note: finally, add a chest at 4,11.
	m_tiles[1][4 + 11 * 16] = 6;

	return true;
}

void Example::update(float deltaT)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && m_window.hasFocus())
	{
		m_running = false;
	}

	ImGui::Begin("Kage2D");
	if(ImGui::Button("Exit"))
	{ 
		m_running = false;
	}
	ImGui::End();
}

void Example::render()
{
	m_window.draw(*m_backgroundSprite);
	tileMap.render(m_window);
	// Note: render layer 0 (background)
	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			m_atlas.draw(m_window, x * 32, y * 32, m_tiles[0][x + y * 16]);
		}
	}
	// Note: render layer 1 (foreground)
	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			m_atlas.draw(m_window, x * 32, y * 32, m_tiles[1][x + y * 16]);
		}
	}

	// The next line draws the physics debug info. This should be removed in a final release.
	kage::Physics::debugDraw(&m_window, 64);
}

void Example::cleanup()
{
	delete m_backgroundSprite;
}

