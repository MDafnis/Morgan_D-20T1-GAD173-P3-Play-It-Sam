////////////////////////////////////////////////////////////
// KAGE2D
// Copyright (C) 2019 Kojack (rajetic@gmail.com)
//
// KAGE2D is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#include "kage2dutil/sfml_util.h"

namespace kage
{
	void selectSpriteTile1D(sf::Sprite *sprite, unsigned int tileNum, unsigned int tileWidth, unsigned int tileHeight)
	{
		int tileColumns = sprite->getTexture()->getSize().x / tileWidth;
		sprite->setTextureRect(sf::IntRect((tileNum % tileColumns)*tileWidth, (tileNum / tileColumns)*tileHeight, tileWidth, tileHeight));
	}

	void selectSpriteTile2D(sf::Sprite *sprite, unsigned int tileX, unsigned int tileY, unsigned int tileWidth, unsigned int tileHeight)
	{
		sprite->setTextureRect(sf::IntRect(tileX*tileWidth, tileY*tileHeight, tileWidth, tileHeight));
	}

	void centreOrigin(sf::Sprite *sprite)
	{
		sf::IntRect rect = sprite->getTextureRect();
		sprite->setOrigin(rect.width / 2.0f, rect.height / 2.0f);
	}

	void drawLine(sf::RenderWindow &win, kf::Vector2 p1, kf::Vector2 p2, sf::Color c)
	{
		drawLine(win, p1, p2, c, c);
	}
	
	void drawLine(sf::RenderWindow &win, kf::Vector2 p1, kf::Vector2 p2, sf::Color c1, sf::Color c2)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(kf::convertVector2<sf::Vector2f>(p1), c1),
			sf::Vertex(kf::convertVector2<sf::Vector2f>(p2), c2)
		};
		win.draw(line, 2, sf::LineStrip);
	}

	void drawBox(sf::RenderWindow &win, kf::Vector2 p1, kf::Vector2 p2, sf::Color c)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(p1), c),
			sf::Vertex(sf::Vector2f(p2.x, p1.y), c),
			sf::Vertex(sf::Vector2f(p2), c),
			sf::Vertex(sf::Vector2f(p1.x, p2.y), c),
			sf::Vertex(sf::Vector2f(p1), c),
		};
		win.draw(line, 5, sf::LineStrip);
	}

	void drawCircle(sf::RenderWindow &win, kf::Vector2 p, float radius, unsigned int segments, sf::Color c)
	{
		sf::Vertex lines[100];
		if (segments > 99)
			segments = 99;
		for (unsigned int i = 0; i < segments; ++i)
		{
			float angle = float((3.14159265*2.0 / segments)*i);
			sf::Vector2f pos(cos(angle)*radius + p.x, sin(angle)*radius + p.y);
			lines[i].position = pos;
			lines[i].color = c;
		}
		lines[segments] = lines[0];
		win.draw(lines, segments+1, sf::LineStrip);
	}

	void drawArrow(sf::RenderWindow &win, kf::Vector2 p1, kf::Vector2 p2, float headWidth, float headLength, sf::Color c)
	{
		kf::Vector2 dir = p2 - p1;
		dir.normalise();
		kf::Vector2 perp = dir.perpCW();

		sf::Vertex line[] =
		{
			sf::Vertex(p1, c),
			sf::Vertex(p2, c),
			sf::Vertex(p2, c),
			sf::Vertex(p2-dir*headLength+perp*headWidth, c),
			sf::Vertex(p2, c),
			sf::Vertex(p2 - dir * headLength - perp * headWidth, c)
		};
		win.draw(line, 6, sf::Lines);
	}

}