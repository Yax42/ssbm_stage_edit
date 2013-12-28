#include "MapHead.h"

int									*MapHead::m_ptr = NULL;
std::vector<Map::ObjectGroup *>		MapHead::m_groups;

void		MapHead::load(int *ptr)
{
	m_ptr = ptr;
	int		i = 0;
	for (int *ptrToGroup = ptr; *ptr != 0; ptr += 2)
	{
		m_groups.push_back(new Map::ObjectGroup(&ptrToGroup[0], &ptrToGroup[1]));
		i++;
	}
}

void		MapHead::clean()
{
	if (m_groups.size() > 0)
	{
		for (unsigned int i = 0; i < m_groups.size(); i++)
			delete m_groups[i];
		m_groups.clear();
	}
}

//----------------------------------------------------------

namespace Map
{
	ObjectGroup::ObjectGroup(int *ptr, int *id, int deep) : Object(deep)
	{
		m_ptr = ptr;
		m_id = id;
		m_ptrToHeadNode = Data::get<int>(*m_ptr);
		m_unknown1 = Data::get<int>(*m_ptr, 1);
		m_unknown2 = Data::get<int>(*m_ptr, 2);
		m_node = new Node(m_ptrToHeadNode, 1);
	}

	ObjectGroup::~ObjectGroup()
	{
		delete m_node;
	}


//----------------------------------------------------------
	Node::Node(int *ptr, int deep) : Object(deep)
	{
		m_ptr = ptr;

		m_intData = Data::get<int>(*ptr);
		m_floatData = Data::get<float>(*ptr);

		if (m_intData[CHILD_PTR] != 0)
			m_child = new Node(&m_intData[CHILD_PTR], deep + 1);
		else
			m_child = NULL;
		if (m_intData[NEXT_PTR] != 0)
			m_next = new Node(&m_intData[NEXT_PTR], deep);
		else
			m_next = NULL;
	}

	Node::~Node()
	{
		if (m_next != NULL)
			delete m_next;
		if (m_child != NULL)
			delete m_child;
	}
}