#pragma once

#include "SFML\Graphics.hpp"
#include "Object.h"
#include "Window.h"
#include "Coll_Node.h"
#include "Coll_LinkNode.h"
#include "Coll_ElemNode.h"

class CollData : Ptr
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
	static const std::vector<std::string> m_labels;
private:
	CollData(int *ptr);
	void	load();
public:
	~CollData();
public:
	void			print();
	virtual void	display();
	void			autoResolve();
public:
	int									m_count;
	int									m_countLink;
	int									m_countElem;
	std::vector<Coll::Node *>			m_nodes;
	std::vector<Coll::LinkNode *>		m_links;
	std::vector<Coll::ElemNode *>		m_elems;
public:
	static CollData					*Instance;
	static void							init(int *ptr);
};