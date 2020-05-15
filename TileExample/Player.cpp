#include "Player.h"
#include "example.h"

Player::Player()
{
	m_sprite = kage::TextureManager::getSprite("data/zazaka.png");
	m_sprite->scale(0.5, 0.5);
	kage::centreOrigin(m_sprite);
	m_tags.add("Player");

	m_physicsStyle = GameObject::e_psBox2D;

	// Make a Box2D body
	m_body = kage::Physics::BodyBuilder()
		.pos(0, 0)
		.userData(this)	// This lets the body know which GameObject owns it
		.build();

	// Make a fixture (collision shape) for the body
	kage::Physics::CircleBuilder()
		.radius(0.4)
		.mass(1)
		.userData(0) // main body
		.build(m_body); // We need to tell the builder which body to attach to

	kage::Physics::CircleBuilder()
		.radius(0.05)
		.mass(0.0001)
		.pos(0, 0.4)// the main body size is 0.4 radius, meaning the position will need to move to the bottom of the main body.
		.sensor(true) // sensor stops the collider from colliding
		.userData(1) // feet
		.build(m_body); // We need to tell the builder which body to attach to

	kage::Physics::CircleBuilder()
		.radius(0.05)
		.mass(0.0001)
		.pos(-0.4, 0)// the main body size is 0.4 radius, meaning the position will need to move to the bottom of the main body.
		.sensor(true) // sensor stops the collider from colliding
		.userData(2) // left collider
		.build(m_body); // We need to tell the builder which body to attach to
	kage::Physics::CircleBuilder()
		.radius(0.05)
		.mass(0.0001)
		.pos(0.4, 0)// the main body size is 0.4 radius, meaning the position will need to move to the bottom of the main body.
		.sensor(true) // sensor stops the collider from colliding
		.userData(3) // body collider
		.build(m_body); // We need to tell the builder which body to attach to

	kage::Physics::CircleBuilder()
		.radius(0.05)
		.mass(0.0001)
		.pos(0, -0.4)// the main body size is 0.4 radius, meaning the position will need to move to the bottom of the main body.
		.sensor(true) // sensor stops the collider from colliding
		.userData(4) // head collider
		.build(m_body); // We need to tell the builder which body to attach to
}

void Player::update(float deltaT)
{
	bool onGround = kage::Physics::getTouching(m_body, 1);
	bool onWallL = kage::Physics::getTouching(m_body, 2);
	bool onWallR = kage::Physics::getTouching(m_body, 3);
	bool onWallU = kage::Physics::getTouching(m_body, 4);
	bool moving = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
	bool moveLeft = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
	bool moveRight = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
	bool currentUp = (sf::Keyboard::isKeyPressed(sf::Keyboard::Up));

	kf::Vector2 v = velocity();
	if (onGround)
	{
		v.x *= 0.2;
		leapingFromWall = false;
		timer = 0.15;
	}
	else if (!onGround && !leapingFromWall) {
		timer -= deltaT;
		if (timer < 0)
		{
			m_body->SetGravityScale(1);
		}
		else {
			v.y = 0;
			velocity(v);
			m_body->SetGravityScale(0);
		}
	}
	if (moveLeft && !onWallL | !onWallR | !onWallU)
	{
		if (leapingFromWall)
		{
			v.x = -6;
		}
		else
		{
			v.x = -8;
		}
	}
	if (moveRight && !onWallL | !onWallR | !onWallU)
	{
		if (leapingFromWall)
		{
			v.x = 6;
		}
		else
		{
			v.x = 8;
		}
	}
	if (!lastUp && currentUp && onGround) // !lastUp && currentUp = key pressed, lastUp && !currentUp = key released.
	{
		v.y = -5;
	}
	if (onWallL && moveRight)
	{
		v.y = -8;
		v.x = 8;
		leapingFromWall = true;
	}
	if (onWallR && moveLeft)
	{
		v.y = -8;
		v.x = -8;
		leapingFromWall = true;
	}
	if (onWallL && moveLeft && !moveRight) {
		v.y = 0;
		m_body->SetGravityScale(0);
	}
	else
	{
		m_body->SetGravityScale(1);
	}
	if (onWallR && moveRight && !moveLeft) {
		v.y = 0;
		m_body->SetGravityScale(0);
	}
	else
	{
		m_body->SetGravityScale(1);
	}
	if (onWallU && currentUp) {
		v.y = -0.1;
		v.x = 0;
		m_body->SetGravityScale(0);
		if (moveRight)
		{
			v.x = 8;
			v.y = 4;
		}
		if (moveLeft)
		{
			v.x = -8;
			v.y = 4;
		}
		if (moveRight | moveLeft)
		{
			m_body->SetGravityScale(1);
		}
	}
	else
	{
		m_body->SetGravityScale(1);
	}
	lastUp = currentUp;
	velocity(v);

	if (Example::inst().lives <= 0)
	{
		Example::inst().lives = 0;
		m_dead = true;
	}
	if (deathTick) {
		deathTimer -= deltaT;
	}
	if (deathTimer <= 0)
	{
		deathTick = false;
	}
}

void Player::onCollision(GameObject* obj)
{
	//if (obj->m_tags.has("enemy"))
	//{
		//m_dead = true;		// kills itself
		//obj->m_dead = true;	// kills the other object
	//}
}

void Player::onCollision(b2Fixture* fix)
{
	if ((int)(fix->GetUserData()) == 4) // Fake ID value 1234
	{
		if (!deathTick)
		{
			position(2, 4);
			deathTimer = 1;
			Example::inst().lives--;
			deathTick = true;
		}
	}
}