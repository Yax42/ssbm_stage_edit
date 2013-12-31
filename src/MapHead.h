#pragma once

#include "SFML\Graphics.hpp"
#include "Object.h"
#include "Window.h"
#include "Map_Group.h"

class MapHead
{
private:
	MapHead();
	~MapHead();
public:
	static void			load(int *ptr);
	static void			print();
	static void			display();
	static void			clean();
	static Map::Node	*getMouseTarget();
public:
	static int									*m_ptr;
	static std::vector<Map::ObjectGroup *>		m_groups;
};