#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>
#include "kf/kf_log.h"
#include "kf/kf_vector2.h"
#include "kf/kf_streams.h"
#include "kage2dutil/imgui.h"
#include "kage2dutil/imgui-SFML.h"
#include "kage2dutil/sfml_util.h"
#include "kage2dutil/gameobject.h"
#include "kage2dutil/world.h"
#include "kage2dutil/texture_manager.h"
#include "kage2dutil/sound_manager.h"

class App
{
protected:
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	sf::Font m_font;
	bool m_running;

public:
	App();
	virtual ~App();
	virtual bool init();
	virtual bool start();
	virtual void update(float deltaT);
	virtual void render();
	virtual void run();
	virtual void cleanup();

	sf::RenderWindow &getWindow();
	sf::Clock &getClock();

};
