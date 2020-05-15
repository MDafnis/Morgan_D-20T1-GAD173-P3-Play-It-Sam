////////////////////////////////////////////////////////////
// KAGE2D
// Copyright (C) 2019 Kojack (rajetic@gmail.com)
//
// KAGE2D is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////
 
#include "kage2dutil/gameobject.h"

namespace kage
{
	void GameObject::preUpdate(float dt)
	{
		if (m_life > 0)
		{
			m_life -= dt;
			if (m_life <= 0)
			{
				m_dead = true;
				return;
			}
		}
		if (m_physicsStyle == e_psNewtonian)
		{
			if (m_gravity)
				m_velocity += World::gravity() * dt;
			m_velocity += m_force * dt;
			m_position += m_velocity * dt;
			m_force.set(0, 0);
		}
		else
			if (m_physicsStyle == e_psBox2D)
			{
				m_position.set(m_body->GetPosition().x * kage::World::scale(), m_body->GetPosition().y * kage::World::scale());
				m_rotation = m_body->GetAngle();
			}
	}

	void GameObject::update(float dt)
	{
	}

	void GameObject::render(sf::RenderWindow &rw)
	{
		if(m_sprite)
		{
			m_sprite->setPosition(m_position.x, m_position.y);
			m_sprite->setRotation(m_rotation.degrees());
			rw.draw(*m_sprite);
		}
	}

	GameObject::GameObject()
	{
		m_life = 0;
		m_dead = false;
		m_position.set(0,0);
		m_velocity.set(0,0);
		m_force.set(0,0);
		m_physicsStyle = e_psNone;
		m_sprite = 0;
		m_id = 0;
		m_collide = true;
		m_body = 0;
		m_rotation = 0;
		m_gravity = true;
		m_order = 0;
	}

	GameObject::~GameObject()
	{
		if(m_body)
		{
			m_body->GetWorld()->DestroyBody(m_body);
			m_body = 0;
		}
		//World::remove(this);
		delete m_sprite;
		m_sprite = 0;
	}

	GameObject *GameObject::build()
	{
		return new GameObject;
	}

	GameObject &GameObject::position(float x, float y)
	{
		return position(kf::Vector2(x, y));
	}

	GameObject &GameObject::position(const kf::Vector2 &pos)
	{
		m_position = pos;
		if(m_physicsStyle == e_psBox2D)
		{
			m_body->SetTransform(m_position, m_rotation.radians());
		}

		return *this;
	}

	GameObject &GameObject::velocity(float x, float y)
	{
		return velocity(kf::Vector2(x, y));
	}

	GameObject &GameObject::velocity(const kf::Vector2 &vel)
	{
		m_velocity = vel;
		if(m_physicsStyle == e_psBox2D)
		{
			m_body->SetLinearVelocity(m_velocity);
		}
		return *this;
	}

	GameObject &GameObject::addForce(float x, float y)
	{
		return addForce(kf::Vector2(x, y));
	}

	GameObject &GameObject::addForce(const kf::Vector2 &force)
	{
		if (m_physicsStyle == e_psNewtonian)
		{
			m_force += force;
		}
		else
		if(m_physicsStyle == e_psBox2D)
		{
			m_body->ApplyForce(force,b2Vec2(0,0),true);
		}
		return *this;
	}

	kf::Vector2 GameObject::position()
	{
		if (m_physicsStyle == e_psBox2D)
		{
			return m_body->GetTransform().p;
		}
		return m_position;
	}

	kf::Vector2 GameObject::velocity()
	{
		if (m_physicsStyle == e_psBox2D)
		{
			return m_body->GetLinearVelocity();
		}
		return m_velocity;
	}

	void GameObject::onCollision(GameObject *obj)
	{

	}

	void GameObject::onCollision(b2Fixture *fix)
	{

	}

	GameObject &GameObject::addImpulse(const kf::Vector2 &force)
	{
		return addImpulse(force.x, force.y);
	}
	
	GameObject &GameObject::addImpulse(float x, float y)
	{
		if (m_physicsStyle == e_psBox2D)
		{
			m_body->ApplyLinearImpulse(b2Vec2(x,y), b2Vec2(0, 0), true);
		}
		return *this;
	}

	void GameObject::property(std::string name, int &p)
	{
		m_propertiesInt.push_back(std::pair<std::string, int*>(name, &p));
	}

	void GameObject::property(std::string name, float &p)
	{
		m_propertiesFloat.push_back(std::pair<std::string, float*>(name, &p));
	}

	void GameObject::property(std::string name, bool &p)
	{
		m_propertiesBool.push_back(std::pair<std::string, bool*>(name, &p));
	}
}
