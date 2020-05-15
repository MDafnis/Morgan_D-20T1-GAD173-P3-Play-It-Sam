////////////////////////////////////////////////////////////
// KAGE2D
// Copyright (C) 2019 Kojack (rajetic@gmail.com)
//
// KAGE2D is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#pragma once
#include <SFML/Audio.hpp>

namespace kage
{
	namespace SoundManager
	{
		sf::Sound *playSound(const std::string &filename);
		void update();
	}
}

