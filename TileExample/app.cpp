#include "app.h"

#include "SFML/Graphics.hpp"
#include "kage2dutil/system.h"
#include "kage2dutil/imgui.h"
#include "kage2dutil/imgui-SFML.h"
#include "kage2dutil/physics.h"

App::App()
{
}

App::~App()
{
}

bool App::init()
{
	// This sets the working directory to where the executable is.
	kage::initDirectory();

	// Start the KF log system, tell it to log to the console and a file.
	kf::LogSystem::getDefault().addCout();
	kf::LogSystem::getDefault().addFile("base.log");
	kf_log("Started");

	m_window.create(sf::VideoMode(1920, 1080, 32), "Kage2D");
	m_window.setFramerateLimit(60);
	
	if (!m_font.loadFromFile("data/bluehigh.ttf"))
	{
		return false;
	}
	ImGui::CreateContext();
	ImGui::SFML::Init(m_window);
	
	// Initialise the physics system. Set the default gravity to 9.8m/s^2 down.
	kage::Physics::init(b2Vec2(0, 9.8));

	return true;
}

bool App::start()
{
	return true;
}

void App::update(float deltaT)
{

}

void App::render()
{

}

void App::run()
{
	m_running = true;

	if (!init())
	{
		return;
	}

	if (!start())
	{
		return;
	}

	while (m_window.isOpen() && m_running)
	{
		// Events are things such as keys being pressed, the window closing, etc.
		// There could be several events waiting for us, so use a loop to process them all.
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
			{
				m_window.close();
				break;
			}
		}
		sf::Time deltaT_sfml = m_clock.restart();
		float deltaT = deltaT_sfml.asSeconds();
		ImGui::SFML::Update(m_window, deltaT_sfml);

		kage::Physics::update(1.0/60.0);
		kage::World::update(1.0/60.0);
		update(1.0/60.0);

		// Clear the window.
		m_window.clear();

		render();
		kage::World::render(m_window);

		ImGui::SFML::Render(m_window);

		// Calling display will make the contents of the window appear on screen (before this, it was kept hidden in the back buffer).
		m_window.display();
	}
	
	cleanup();
}

void App::cleanup()
{

}

sf::RenderWindow &App::getWindow()
{
	return m_window;
}

sf::Clock &App::getClock()
{
	return m_clock;
}

