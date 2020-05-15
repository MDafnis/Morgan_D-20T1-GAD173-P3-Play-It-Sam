////////////////////////////////////////////////////////////
// KAGE2D
// Copyright (C) 2019 Kojack (rajetic@gmail.com)
//
// KAGE2D is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////
 
#include "kage2dutil/physics.h"
#include "kage2dutil/gameobject.h"
#include "kage2dutil/sfml_util.h"
#include "kf/kf_log.h"

namespace kage
{

	namespace Physics
	{
		struct Collision
		{
			b2Fixture* f1;
			b2Fixture* f2;
			GameObject* g1;
			GameObject* g2;
		};

		std::vector<Collision> g_collisions;

		class ContactListener : public b2ContactListener
		{
		public:
			virtual void BeginContact(b2Contact* contact)
			{
				b2Fixture *f1 = contact->GetFixtureA();
				b2Fixture *f2 = contact->GetFixtureB();
				kf::Vector2 v1 = f1->GetBody()->GetLinearVelocity();
				kf::Vector2 v2 = f2->GetBody()->GetLinearVelocity();
				kf::Vector2 p1 = f1->GetBody()->GetPosition();
				kf::Vector2 p2 = f1->GetBody()->GetPosition();

				b2WorldManifold worldManifold;
				contact->GetWorldManifold(&worldManifold);

				kf::Vector2 norm = worldManifold.normal;
				
				GameObject *g1 = getGameObject(f1);
				GameObject *g2 = getGameObject(f2);

				g_collisions.push_back(Collision{f1,f2,g1,g2});
			}
		};

		class DebugDraw : public b2Draw
		{
		public:
			sf::RenderWindow *m_win;
			float m_scale;
			void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
			{
				sf::Vertex lines[100];
				if (vertexCount > 99)
					vertexCount = 99;
				for (int i = 0; i < vertexCount; ++i)
				{
					lines[i].position.x = vertices[i].x * m_scale;
					lines[i].position.y = vertices[i].y * m_scale;
					lines[i].color = sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
				}
				lines[vertexCount] = lines[0];
				m_win->draw(lines, vertexCount + 1, sf::LineStrip);
			}
			void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
			{
				DrawPolygon(vertices, vertexCount, color);
			}
			void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
			{
				kage::drawCircle(*m_win, (kf::Vector2)center*m_scale, radius*m_scale, 16, sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
			}
			void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
			{
				kage::drawCircle(*m_win, (kf::Vector2)center*m_scale, radius*m_scale, 16, sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
				kage::drawArrow(*m_win, (kf::Vector2)center*m_scale, (kf::Vector2)center*m_scale + (kf::Vector2)axis*m_scale, 0.05f, 0.1f, sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
			}
			void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
			{
				kage::drawLine(*m_win, (kf::Vector2)p1*m_scale, (kf::Vector2)p2*m_scale, sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
			}
			void DrawTransform(const b2Transform& xf)
			{
				kage::drawArrow(*m_win, (kf::Vector2)xf.p*m_scale, (kf::Vector2)xf.p*m_scale + (kf::Vector2)xf.q.GetXAxis()*m_scale, 0.05f, 0.1f, sf::Color(255, 0, 0, 255));
				kage::drawArrow(*m_win, (kf::Vector2)xf.p*m_scale, (kf::Vector2)xf.p*m_scale + (kf::Vector2)xf.q.GetYAxis()*m_scale, 0.05f, 0.1f, sf::Color(0, 255, 0, 255));
			}
			void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {}
		};

		class RayCastCallback : public b2RayCastCallback {
		public:
			Hit hit;

			RayCastCallback()
			{
			}

			float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
			{
				if (!hit.hit || fraction < hit.distance)
				{
					//GameObject *t = (GameObject *)fixture->GetUserData();
					hit.pos.set(point.x, point.y);
					hit.distance = fraction;
					hit.hit = true;
					hit.fixture = fixture;
					return fraction;
				}
				return 1;
			}
		};

		namespace
		{
			b2World *g_world = 0;
			int g_velocityIterations = 8;
			int g_positionIterations = 3;
			ContactListener g_contactListener;
			DebugDraw g_debugDraw;
			b2Body *g_defaultStatic = 0;
			std::vector<b2Fixture *> g_fixturesFromCallback;
		}

		b2World *getWorld()
		{
			return g_world;
		}

		void init(b2Vec2 gravity)
		{
			if (g_world)
			{
				delete g_world;
			}
			g_world = new b2World(gravity);
			g_world->SetContactListener(&g_contactListener);
			g_world->SetDebugDraw(&g_debugDraw);
			g_debugDraw.SetFlags(b2Draw::e_shapeBit);
			g_defaultStatic = createBody(kf::Vector2::ZERO(), 0, b2BodyType::b2_staticBody, true);
		}
		
		b2Body* getDefaultStatic()
		{
			return g_defaultStatic;
		}

		void update(float dt)
		{
			g_collisions.clear();
			g_world->Step(dt, g_velocityIterations, g_positionIterations);
			for (int i = 0; i < g_collisions.size(); ++i)
			{
				if (g_collisions[i].g1)
				{
					if (g_collisions[i].g2)
					{
						g_collisions[i].g1->onCollision(g_collisions[i].g2);
					}
					else
					{
						g_collisions[i].g1->onCollision(g_collisions[i].f2);
					}
				}
				if (g_collisions[i].g2)
				{
					if (g_collisions[i].g1)
					{
						g_collisions[i].g2->onCollision(g_collisions[i].g1);
					}
					else
					{
						g_collisions[i].g2->onCollision(g_collisions[i].f1);
					}
				}
			}
		}

		void clear()
		{
			b2Body *body = g_world->GetBodyList();
			while (body)
			{
				auto next = body->GetNext();
				if (body != g_defaultStatic)
				{
					g_world->DestroyBody(body);
				}
				body = next;
			}
			clearDefaultStaticFixtures();
		}

		void clearDefaultStaticFixtures()
		{
			if (g_defaultStatic)
			{
				b2Fixture *fix = g_defaultStatic->GetFixtureList();
				while (fix)
				{
					auto next = fix->GetNext();
					g_defaultStatic->DestroyFixture(fix);
					fix = next;
				}
			}
		}

		void gravity(b2Vec2 gravity)
		{
			g_world->SetGravity(gravity);
		}
		
		b2Vec2 gravity()
		{
			return g_world->GetGravity();
		}

		void debugDraw(sf::RenderWindow *win, float scale)
		{
			g_debugDraw.m_win = win;
			g_debugDraw.m_scale = kage::World::scale();
			g_world->DrawDebugData();
		}

		b2Fixture *createCircle(b2Body *body, float radius, kf::Vector2f offset, float density, float damping, void *userData, float restitution, float friction)
		{
			b2CircleShape shape;
			shape.m_radius = radius;
			shape.m_p = offset;
			b2FixtureDef fixDef;
			fixDef.density = density;
			fixDef.restitution = restitution;
			fixDef.friction = friction;
			fixDef.shape = &shape;
			fixDef.userData = userData;
			b2Fixture *fixture = body->CreateFixture(&fixDef);
			body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			body->SetLinearDamping(damping);
			return fixture;
		}

		b2Fixture *createBox(b2Body *body, kf::Vector2f size, kf::Vector2f offset, float angle, float density, float damping, void *userData,  float restitution, float friction)
		{
			b2PolygonShape shape;
			shape.SetAsBox(size.x / 2.0f, size.y / 2.0f, offset, angle);
			b2FixtureDef fixDef;
			fixDef.density = density;
			fixDef.restitution = restitution;
			fixDef.friction = friction;
			fixDef.shape = &shape;
			fixDef.userData = userData;
			b2Fixture *fixture = body->CreateFixture(&fixDef);
			body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			body->SetLinearDamping(damping);
			return fixture;
		}

		b2Fixture *createEdge(b2Body *body, kf::Vector2f vertex1, kf::Vector2f vertex2, float density, float damping, void *userData, float restitution, float friction)
		{
			b2EdgeShape shape;
			shape.Set(vertex1, vertex2);
			b2FixtureDef fixDef;
			fixDef.density = density;
			fixDef.restitution = restitution;
			fixDef.friction = friction;
			fixDef.shape = &shape;
			fixDef.userData = userData;
			b2Fixture *fixture = body->CreateFixture(&fixDef);
			body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			body->SetLinearDamping(damping);
			return fixture;
		}

		b2Body *createBody(kf::Vector2f pos, float angle, b2BodyType type, bool fixedRotation, void *userData)
		{
			b2BodyDef bodyDef;
			bodyDef.position = pos;
			bodyDef.type = type;
			bodyDef.angle = angle;
			bodyDef.fixedRotation = fixedRotation;
			bodyDef.userData = userData;
			return g_world->CreateBody(&bodyDef);
		}

		Hit rayCast(kf::Vector2 p1, kf::Vector2 p2)
		{
			RayCastCallback cb;
			g_world->RayCast(&cb, p1, p2);
			return cb.hit;
		}

		GameObject *getGameObject(b2Fixture *fixture)
		{
			if (fixture)
			{
				b2Body *body = fixture->GetBody();
				return (GameObject*)body->GetUserData();
			}
			return 0;
		}

		GameObject *getGameObject(b2Body *body)
		{
			if (body)
			{
				return (GameObject*)body->GetUserData();
			}
			return 0;
		}

		b2Fixture *getTouching(b2Body *body, int fixtureNum)
		{
			b2Fixture *f = body->GetFixtureList();
			int i = 0;
			for (; f; f = f->GetNext(),++i)
			{
				if ((int)f->GetUserData() == fixtureNum)
				{
					break;
				}
			}
			if (!f)
			{
				return 0;
			}
			for (b2ContactEdge* c = body->GetContactList(); c; c = c->next)
			{
				b2Fixture *f1;
				b2Fixture *f2;
				
				if (c->contact->GetFixtureA() == f)
				{
					f1 = c->contact->GetFixtureA();
					f2 = c->contact->GetFixtureB();
				}
				else if(c->contact->GetFixtureB() == f)
				{
					f1 = c->contact->GetFixtureB();
					f2 = c->contact->GetFixtureA();
				}
				else
				{
					continue;
				}

				if (c->contact->IsTouching())
				{
					return f2;
				}
			}
			return 0;
		}

		class AABBCallback :public b2QueryCallback
		{
		public:
			b2Vec2 pos;
			AABBCallback(b2Vec2 p) : pos(p)
			{

			}
			bool ReportFixture(b2Fixture* fixture)
			{
				if (fixture->TestPoint(pos))
				{
					g_fixturesFromCallback.push_back(fixture);
				}
				return true;
			}
		};

		b2Fixture *testPoint(kf::Vector2 pos)
		{
			g_fixturesFromCallback.clear();
			AABBCallback aabbcb(pos);
			b2AABB aabb;
			aabb.lowerBound = b2Vec2(pos);
			aabb.upperBound = b2Vec2(pos);
			g_world->QueryAABB(&aabbcb, aabb);
			if (g_fixturesFromCallback.size() > 0)
			{
				return g_fixturesFromCallback.front();
			}
			return 0;
		}
		

		CircleBuilder &CircleBuilder::radius(float r)
		{
			m_radius = r;
			return *this;
		}

		b2Fixture *CircleBuilder::build(b2Body *body)
		{
			if (m_calcDensity)
			{
				m_density = m_mass / (m_radius * m_radius * 3.14159265);
			}
			b2Fixture *fix = createCircle(body, m_radius, m_pos, m_density, m_damping, m_userData, m_restitution, m_friction);
			fix->SetSensor(m_sensor);
			return fix;
		}

		BoxBuilder &BoxBuilder::size(float x, float y)
		{
			m_size.set(x, y);
			return *this;
		}

		BoxBuilder &BoxBuilder::size(float s)
		{
			m_size.set(s, s);
			return *this;
		}

		BoxBuilder &BoxBuilder::size(const kf::Vector2 &s)
		{
			m_size = s;
			return *this;
		}

		b2Fixture *BoxBuilder::build(b2Body *body)
		{
			if (m_calcDensity)
			{
				m_density = m_mass / (m_size.x * m_size.y);
			}
			b2Fixture *fix = createBox(body, m_size, m_pos, m_angle, m_density, m_damping, m_userData, m_restitution, m_friction);
			fix->SetSensor(m_sensor);
			return fix;

		}

		BodyBuilder &BodyBuilder::type(b2BodyType type)
		{
			m_type = type;
			return *this;
		}

		BodyBuilder &BodyBuilder::pos(const kf::Vector2 &p)
		{
			m_pos = p;
			return *this;
		}

		BodyBuilder &BodyBuilder::pos(float x, float y)
		{
			m_pos.set(x, y);
			return *this;
		}

		BodyBuilder &BodyBuilder::angle(float a)
		{
			m_angle = a;
			return *this;
		}

		BodyBuilder &BodyBuilder::userData(void *ud)
		{
			m_userData = ud;
			return *this;
		}

		BodyBuilder &BodyBuilder::fixedRotation(bool fr)
		{
			m_fixedRotation = fr;
			return *this;
		}

		b2Body *BodyBuilder::build()
		{
			return createBody(m_pos, m_angle, m_type, m_fixedRotation, m_userData);
		}

	}

}
