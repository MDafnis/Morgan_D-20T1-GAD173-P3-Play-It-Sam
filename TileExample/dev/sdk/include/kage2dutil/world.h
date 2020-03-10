////////////////////////////////////////////////////////////
// KAGE2D
// Copyright (C) 2019 Kojack (rajetic@gmail.com)
//
// KAGE2D is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#ifndef QGF2D_WORLD_HEADER
#define QGF2D_WORLD_HEADER
#include "kf/kf_vector2.h"
#include <SFML/Graphics.hpp>
//#include "box2d\Box2D.h"


namespace kage
{
	class GameObject;

	namespace World
	{
		kf::Vector2 gravity();
		void gravity(const kf::Vector2 &grav);
		long long newID();
		void update(float dt);
		void render(sf::RenderWindow &rw);
		void add(GameObject *obj);
		void remove(GameObject *obj);
		void remove(std::string tag);
		void clear();
		//void setOrderDirty();
		void setWorldSize(kf::Vector2 size);
		GameObject *findByID(long long id);
		GameObject *findByTag(std::string tag);
		std::vector<GameObject *> findAllByTag(std::string tag);
		float scale();	// Get the scaling factor between the physics engine and the renderer.
		void scale(float s); // Set the scaling factor between the physics engine and the renderer.
		std::map<long long, GameObject*> &getWorldObjects();
		void inspector();
		void setView(sf::RenderWindow& rw, kf::Vector2 centre, float zoom);
		void setView(sf::RenderWindow& rw);

		template<typename T>
		T *build()
		{
			GameObject *obj = new T;
			obj->m_id = newID();
			add(obj);
			return (T*)obj;
		}
	}
}

#endif