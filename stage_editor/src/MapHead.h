#pragma once

#include "SFML\Graphics.hpp"
#include "Object.h"
#include "Window.h"
#include "Map_Group.h"

class MapHead // : public Ptr
{
private:
	MapHead(int *ptr);
public:
	static void			init(int *ptr);
	void			print();
	void			display();
	~MapHead();
public:
	int									*m_ptr;
	int									*m_deepPtr;
	std::vector<Map::ObjectGroup *>		m_groups;
	static MapHead						*Instance;
};