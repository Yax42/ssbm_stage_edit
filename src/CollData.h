#pragma once

#include "SFML\Graphics.hpp"
#include "Object.h"
#include "Window.h"
#include "Coll_Node.h"

class CollData 
{
public:
	enum
	{
		LOCATIONS,
		NUMBER,
	};
private:
	CollData();
	~CollData();
public:
	static void			load(int *ptr);
	static void			print();
	static void			display();
	static void			clean();
public:
	static int									*m_ptr;
	static int									*m_deepPtr;
	static int									m_count;
	static std::vector<Coll::Node *>			m_nodes;
};