#include "Map_Group.h"

namespace Map
{
	int		ObjectGroup::tmp;
	ObjectGroup::ObjectGroup(int *ptr, int *id, int deep) : Object(deep)
	{
		m_ptr = ptr;
		m_id = id;
		m_ptrToHeadNode = Data::get<int>(*m_ptr);
		m_unknown1 = Data::get<int>(*m_ptr, 1);
		m_unknown2 = Data::get<int>(*m_ptr, 2);
		ObjectGroup::tmp = 0;
		m_node = new Node(m_ptrToHeadNode, 1);
		m_count = ObjectGroup::tmp;
	}

	ObjectGroup::~ObjectGroup()
	{
		delete m_node;
	}

	void		ObjectGroup::print()
	{
		printTab();
		std::cout <<
			"ObjectGroup\tid:" << *m_id <<
			" ptr:" << *m_ptr <<
			" node:" << *m_ptrToHeadNode <<
			" uk1:" << *m_unknown1 <<
			" uk2:" << *m_unknown2 <<
			" count:" << m_count << std::endl;
		m_node->print();
	}

	void		ObjectGroup::display()
	{
		m_node->display();
	}

	Map::Node	*ObjectGroup::getMouseTarget()
	{
		return m_node->getMouseTarget();
	}
}