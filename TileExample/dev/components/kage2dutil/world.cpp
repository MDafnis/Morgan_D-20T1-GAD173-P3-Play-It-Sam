////////////////////////////////////////////////////////////
// KAGE2D
// Copyright (C) 2019 Kojack (rajetic@gmail.com)
//
// KAGE2D is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////
 
#include "kage2dutil/world.h"
#include "kage2dutil/gameobject.h"

namespace kage
{

	namespace World
	{
		const int c_divisions = 10;

		std::map<long long, GameObject *> worldObjects;
		long long worldHighestID = 0;
		kf::Vector2 worldGravity(0,0);
		bool sortOrderDirty = false;
		kf::Vector2 worldSize(1920, 1080);
		float worldScale = 64.0f;

		kf::Vector2 gravity()
		{
			return worldGravity;
		}

		void gravity(const kf::Vector2 &grav)
		{
			worldGravity = grav;
		}

		long long newID()
		{
			return ++worldHighestID;
		}

		void setWorldSize(kf::Vector2 size)
		{
			worldSize = size;
		}

		void update(float dt)
		{
			std::vector<long long> deadobjects;
			for(std::map<long long, GameObject *>::iterator it = worldObjects.begin(); it!= worldObjects.end(); ++it)
			{
				it->second->preUpdate(dt);
				it->second->update(dt);
				if(it->second->m_dead)
				{
					deadobjects.push_back(it->first);
				}
			}

			{
				std::vector<GameObject *> cells[c_divisions][c_divisions];

				int cx = 0;
				int cy = 0;
				int dx = worldSize.x / c_divisions;
				int dy = worldSize.y / c_divisions;

				for(std::map<long long, GameObject *>::iterator it = worldObjects.begin();it != worldObjects.end();it++)
				{
					if(it->second->m_collide && !it->second->m_dead && it->second->m_physicsStyle == kage::GameObject::PhysicsStyle::e_psNewtonian)
					{
						int sx = int((it->second->m_position.x - it->second->m_sprite->getGlobalBounds().width * 0.5)/dx);
						int sy = int((it->second->m_position.y - it->second->m_sprite->getGlobalBounds().height * 0.5)/dy);
						int ex = int((it->second->m_position.x + it->second->m_sprite->getGlobalBounds().width * 0.5)/dx);
						int ey = int((it->second->m_position.y + it->second->m_sprite->getGlobalBounds().height * 0.5)/dy);
						for(int y=sy;y<=ey;y++)
						{
							for(int x=sx;x<=ex;x++)
							{
								if(x>=0 && x< c_divisions && y>=0 && y<c_divisions)
								{
									cells[y][x].push_back(it->second);
								}
							}
						}
					}
				}
				for(int y=0;y<c_divisions;y++)
				{
					for(int x=0;x<c_divisions;x++)
					{
						for(int i=0;i<cells[y][x].size();i++)
						{
							for(int j=i;j<cells[y][x].size();j++)
							{
								if(i!=j && !cells[y][x][i]->m_dead && !cells[y][x][j]->m_dead)
								{
									if(cells[y][x][i]->m_position.x + cells[y][x][i]->m_sprite->getGlobalBounds().width * 0.35f > cells[y][x][j]->m_position.x - cells[y][x][j]->m_sprite->getGlobalBounds().width * 0.35f
										&& cells[y][x][i]->m_position.x - cells[y][x][i]->m_sprite->getGlobalBounds().width * 0.35f < cells[y][x][j]->m_position.x + cells[y][x][j]->m_sprite->getGlobalBounds().width * 0.35f
										&& cells[y][x][i]->m_position.y + cells[y][x][i]->m_sprite->getGlobalBounds().height * 0.35f > cells[y][x][j]->m_position.y - cells[y][x][j]->m_sprite->getGlobalBounds().height * 0.35f
										&& cells[y][x][i]->m_position.y - cells[y][x][i]->m_sprite->getGlobalBounds().height * 0.35f < cells[y][x][j]->m_position.y + cells[y][x][j]->m_sprite->getGlobalBounds().height * 0.35f)
									{
										cells[y][x][i]->onCollision(cells[y][x][j]);
										cells[y][x][j]->onCollision(cells[y][x][i]);
									}
								}
							}
						}
					}
				}
			}


			for(unsigned int i=0;i<deadobjects.size();++i)
			{
				std::map<long long, GameObject *>::iterator it = worldObjects.find(deadobjects[i]);
				if(it!=worldObjects.end())
				{
					delete it->second;
					worldObjects.erase(it);
				}
			}
		}
		
		bool sortOrder(GameObject *g1, GameObject *g2)
		{
			return g1->m_order < g2->m_order;
		}
		
		void render(sf::RenderWindow &rw)
		{
			std::vector<GameObject *> objects(worldObjects.size());
			int i = 0;
			for(std::map<long long, GameObject *>::iterator it = worldObjects.begin(); it!= worldObjects.end(); ++it)
			{
				objects[i++] = it->second;
			}

			std::sort(objects.begin(), objects.end(), sortOrder);

			for (i = 0; i < objects.size(); ++i)
			{
				objects[i]->render(rw);
			}
		}

		void add(GameObject *obj)
		{
			if (obj->m_id == 0)
			{
				obj->m_id = newID();
			}
			auto it = worldObjects.find(obj->m_id);
			if (it == worldObjects.end())
			{
				worldObjects[obj->m_id] = obj;
			}
		}

		void remove(GameObject *obj)
		{
			auto it = worldObjects.find(obj->m_id);
			if (it != worldObjects.end())
			{
				delete it->second;
				worldObjects.erase(it);
			}
		}

		void remove(std::string tag)
		{
			for (std::map<long long, GameObject *>::iterator it = worldObjects.begin(); it != worldObjects.end();)
			{
				if (it->second->m_tags.has(tag))
				{
					delete it->second;
					it = worldObjects.erase(it);
				}
				else
				{
					it++;
				}
			}
		}

		void clear()
		{
			for (std::map<long long, GameObject *>::iterator it = worldObjects.begin(); it != worldObjects.end();++it)
			{
				delete it->second;
			}
			worldObjects.clear();
		}

		GameObject *findByID(long long id)
		{
			auto it = worldObjects.find(id);
			if (it != worldObjects.end())
				return it->second;
			return 0;
		}

		GameObject *findByTag(std::string tag)
		{
			for (std::map<long long, GameObject *>::iterator it = worldObjects.begin(); it != worldObjects.end(); ++it)
			{
				if (it->second->m_tags.has(tag))
					return it->second;
			}
			return 0;
		}

		std::vector<GameObject *> findAllByTag(std::string tag)
		{
			std::vector<GameObject *> found;
			for (std::map<long long, GameObject *>::iterator it = worldObjects.begin(); it != worldObjects.end(); ++it)
			{
				if (it->second->m_tags.has(tag))
					found.push_back(it->second);
			}
			return found;
		}

		float scale()
		{
			return worldScale;
		}

		void scale(float s)
		{
			worldScale = s;
		}

		std::map<long long, GameObject*>& getWorldObjects()
		{
			return worldObjects;
		}

		void inspector()
		{
			static int currentListItem = 0;
			std::vector<char*> items;

			ImGui::Begin("World Inspector");
			ImGui::Columns(2);

			if (!worldObjects.empty())
			{
				if (currentListItem >= worldObjects.size())
				{
					currentListItem = worldObjects.size() - 1;
				}
				std::vector<long long> ids(worldObjects.size());
				int i = 0;
				for (auto it = worldObjects.begin(); it != worldObjects.end(); ++it)
				{
					ids[i] = it->first;
					std::string s;
					s = std::to_string(it->first);
					if (!it->second->m_tags.getVector().empty())
					{
						s += "  (";
						for (int t = 0; t < it->second->m_tags.getVector().size(); ++t)
						{
							if (t != 0) s += ", ";
							s += it->second->m_tags.getVector()[t];
						}
						s += ")";
					}
					char* text = new char[s.size() + 1];
					sprintf(text, "%s", s.c_str());
					items.push_back(text);
					++i;
				}
				ImGui::ListBox("", &currentListItem, &items[0], items.size());
				for (int ii = 0; ii < items.size(); ++ii)
				{
					delete items[ii];
				}
				ImGui::NextColumn();
				kage::GameObject* gob = kage::World::findByID(ids[currentListItem]);
				if (gob)
				{
					ImGui::Text("GameObject");
					char buf[1000];
					kf::Vector2 pos = gob->position();
					bool changed = false;
					changed = ImGui::SliderFloat("Pos X", &pos.x, 0, 40);
					changed |= ImGui::SliderFloat("Pos Y", &pos.y, 0, 40);
					if (changed)
					{
						gob->position(pos);
					}
					
					kf::Vector2 vel = gob->velocity();
					changed = ImGui::InputFloat2("Velocity", &vel.x);
					if (changed)
					{
						gob->velocity(vel);
					}
					ImGui::InputFloat("Life", &gob->m_life);
					ImGui::Text("");
					ImGui::Text("Custom Properties");
					for (int i = 0; i < gob->m_propertiesInt.size(); ++i)
					{
						ImGui::InputInt(gob->m_propertiesInt[i].first.c_str(), gob->m_propertiesInt[i].second);
					}
					for (int i = 0; i < gob->m_propertiesFloat.size(); ++i)
					{
						ImGui::InputFloat(gob->m_propertiesFloat[i].first.c_str(), gob->m_propertiesFloat[i].second);
					}
					for (int i = 0; i < gob->m_propertiesBool.size(); ++i)
					{
						ImGui::Checkbox(gob->m_propertiesBool[i].first.c_str(), gob->m_propertiesBool[i].second);
					}


					if (gob->m_body)
					{
						ImGui::Text("");
						ImGui::Text("Physics Body");
						float mass = gob->m_body->GetMass();
						if (ImGui::Button("Wake"))
						{
							gob->m_body->SetAwake(true);
						}
						if (ImGui::InputFloat("Mass", &mass))
						{

//							gob->m_body->SetMassData(
						}
						b2Fixture* fix = gob->m_body->GetFixtureList();
						while (fix)
						{
							std::string s;
							switch (fix->GetType())
							{
							case b2Shape::e_circle:
								s += "Circle ";
								break;
							case b2Shape::e_polygon:
								s += "Polygon ";
								break;
							case b2Shape::e_chain:
								s += "Chain ";
								break;
							case b2Shape::e_edge:
								s += "Edge ";
								break;
							}
							s += "UserData:";
							s += std::to_string((unsigned int)fix->GetUserData());
							//s += " Pos:(";
							//s+= std::to_string(fix->GetShape()->
							b2MassData md;
							fix->GetMassData(&md);
							s += " Weight:" + std::to_string(md.mass)+"kg";
							if (fix->IsSensor())
							{
								s += " Sensor";
							}
							ImGui::Text(s.c_str());
							fix = fix->GetNext();
						}

					}
				}

			}
			ImGui::End();
		}

		void setView(sf::RenderWindow &rw, kf::Vector2 centre, float zoom)
		{
			sf::View v = rw.getDefaultView();
			v.setCenter(centre * worldScale);
			v.zoom(zoom);
			rw.setView(v);
		}

		void setView(sf::RenderWindow& rw)
		{
			sf::View v = rw.getDefaultView();
			rw.setView(v);
		}
	}
}
