#include "myGameObject.h"
#include "example.h"

myGameObject::myGameObject()
{
	m_sprite = kage::TextureManager::getSprite("data/zazaka.png");
	kage::centreOrigin(m_sprite);
	m_tags.add("myGameObject");

	m_physicsStyle = GameObject::e_psNewtonian;
}

myGameObject::~myGameObject()
{

}

void myGameObject::update(float deltaT)
{
	// Do logic here
}

void myGameObject::onCollision(GameObject *obj)
{
	//if (obj->m_tags.has("enemy"))
	//{
		//m_dead = true;		// kills itself
		//obj->m_dead = true;	// kills the other object
	//}
}
