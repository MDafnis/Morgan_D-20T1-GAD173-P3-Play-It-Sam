////////////////////////////////////////////////////////////
// KAGE2D
// Copyright (C) 2019 Kojack (rajetic@gmail.com)
//
// KAGE2D is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#ifndef QGF2D_SFML_UTIL_HEADER
#define QGF2D_SFML_UTIL_HEADER
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "kf/kf_vector2.h"

namespace kage
{
	void selectSpriteTile1D(sf::Sprite *sprite, unsigned int tileNum, unsigned int tileWidth, unsigned int tileHeight);
	void selectSpriteTile2D(sf::Sprite *sprite, unsigned int tileX, unsigned int tileY, unsigned int tileWidth, unsigned int tileHeight);
	void centreOrigin(sf::Sprite *sprite);
	void drawLine(sf::RenderWindow &win, kf::Vector2 p1, kf::Vector2 p2, sf::Color c = sf::Color::White);
	void drawLine(sf::RenderWindow &win, kf::Vector2 p1, kf::Vector2 p2, sf::Color c1, sf::Color c2);
	void drawBox(sf::RenderWindow &win, kf::Vector2 p1, kf::Vector2 p2, sf::Color c = sf::Color::White);
	void drawCircle(sf::RenderWindow &win, kf::Vector2 p, float radius, unsigned int segments, sf::Color c = sf::Color::White);
	void drawArrow(sf::RenderWindow &win, kf::Vector2 p1, kf::Vector2 p2, float headWidth, float headLength, sf::Color c = sf::Color::White);
}
#endif
