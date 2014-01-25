#pragma once

#include "SFML\Graphics.hpp"
#include "Object.h"
#include "Window.h"
#include "Coll_Node.h"
#include "Coll_LinkNode.h"
#include "Coll_ElemNode.h"

class CollData 
{
public:
	enum
	{
		LOCATIONS,
		NUMBER,
		LINKS,
		LINK_COUNT,
		REF_TOP,
		REF_BOT,
		REF_RIGHT,
		REF_LEFT,
		M2,
		ELEMS,
		ELEM_COUNT,
		MISTERY,
		COUNT = 12
	};
private:
	CollData();
	~CollData();
public:
	static void			load(int *ptr);
	static void			print();
	static void			display();
	static void			clean();
	static void			autoResolve();
public:
	static int									*m_ptr;
	static int									*m_deepPtr;
	static int									m_count;
	static int									m_countLink;
	static int									m_countElem;
	static std::vector<Coll::Node *>			m_nodes;
	static std::vector<Coll::LinkNode *>		m_links;
	static std::vector<Coll::ElemNode *>		m_elems;
};