#pragma once

#include "SFML\Graphics.hpp"
#include "Object.h"
#include "Window.h"
#include "Map_Group.h"

class MapHead : public Ptr
{
private:
	MapHead();
	~MapHead();
public:
	static void			load(int *ptr);
	static void			print();
	static void			display();
	static void			clean();
public:
	static int									*m_ptr;
	static int									*m_deepPtr;
	static std::vector<Map::ObjectGroup *>		m_groups;
};