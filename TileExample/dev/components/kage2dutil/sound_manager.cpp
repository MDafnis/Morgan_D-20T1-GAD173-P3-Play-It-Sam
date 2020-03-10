////////////////////////////////////////////////////////////
// KAGE2D
// Copyright (C) 2019 Kojack (rajetic@gmail.com)
//
// KAGE2D is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#include <map>
#include <string>
#include "sound_manager.h"
#include "kage2dutil/system.h"

namespace
{
	long long g_maxId=0;
	std::map<std::string, sf::SoundBuffer *> g_soundBuffers;
	std::vector<sf::Sound *> g_sounds;
}
namespace kage
{
	namespace SoundManager
	{
		sf::Sound *playSound(const std::string &filename)
		{
			sf::SoundBuffer *sb;
			std::map<std::string, sf::SoundBuffer *>::iterator it = g_soundBuffers.find(filename);
			if (it == g_soundBuffers.end())
			{
				sb = new sf::SoundBuffer;
				kage::initDirectory();
				if (!sb->loadFromFile(filename))
				{
					delete sb;
					return 0;
				}
				g_soundBuffers[filename] = sb;
			}
			else
			{
				sb = it->second;
			}

			sf::Sound *s = new sf::Sound(*sb);
			s->play();
			return s;
		}

		void update()
		{
			for (int i = 0; i < g_sounds.size(); ++i)
			{
				if (g_sounds[i]->getStatus() == sf::SoundSource::Stopped)
				{
					delete g_sounds[i];
					g_sounds.erase(g_sounds.begin() + i);
					--i;
				}
			}
		}
	}
}
