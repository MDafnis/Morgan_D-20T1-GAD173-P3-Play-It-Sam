////////////////////////////////////////////////////////////
// KAGE2D
// Copyright (C) 2019 Kojack (rajetic@gmail.com)
//
// KAGE2D is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#ifndef QGF2D_GAMEOBJECT_HEADER
#define QGF2D_GAMEOBJECT_HEADER
#include "kf/kf_vector2.h"
#include <SFML/Graphics.hpp>
#include "box2d\Box2D.h"
#include "world.h"
#include "kf/kf_euler.h"
#include "kf/kf_tag.h"

namespace kage
{
	class GameObject
	{
	public:
		enum PhysicsStyle
		{
			e_psNone=0,
			e_psNewtonian,
			e_psBox2D
		};
		kf::Vector2 m_position;
		kf::Vector2 m_velocity;
		kf::Vector2 m_force;
		kf::Euler m_rotation;
		float m_life;
		bool m_dead;
		long long m_id;
		PhysicsStyle m_physicsStyle;
		bool m_collide;
		bool m_gravity;
		sf::Sprite *m_sprite;
		b2Body *m_body;
		int m_order;
		kf::Tags m_tags;
		std::vector<std::pair<std::string, int*>> m_propertiesInt;
		std::vector<std::pair<std::string, float*>> m_propertiesFloat;
		std::vector<std::pair<std::string, bool*>> m_propertiesBool;

		GameObject();
		virtual ~GameObject();
		virtual void preUpdate(float dt);
		virtual void update(float dt);
		virtual void render(sf::RenderWindow &rw);
		virtual void onCollision(GameObject *obj);
		virtual void onCollision(b2Fixture *fix);
		virtual GameObject &position(const kf::Vector2 &pos);
		virtual GameObject &velocity(const kf::Vector2 &vel);
		virtual GameObject &position(float x, float y);
		virtual GameObject &velocity(float x, float y);
		virtual kf::Vector2 position();
		virtual kf::Vector2 velocity();
		virtual GameObject &addForce(const kf::Vector2 &force);
		virtual GameObject &addForce(float x, float y);
		virtual GameObject &addImpulse(const kf::Vector2 &force);
		virtual GameObject &addImpulse(float x, float y);
		static GameObject *build();
		void property(std::string name, int &p);
		void property(std::string name, float &p);
		void property(std::string name, bool &p);
	};
}

#endif