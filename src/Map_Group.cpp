#include "Map_Group.h"

namespace Map
{
	bool	ObjectGroup::Dummy;
	int		ObjectGroup::tmp;

	ObjectGroup::ObjectGroup(int *ptr, int *id, int deep) : Object(deep)
	{
		static int		prev = 0;
		m_ptr = ptr;
		m_id = id;
		m_ptrToHeadNode = Data::get<int>(*m_ptr);
		m_unknown1 = Data::get<int>(*m_ptr, 1);
		m_unknown2 = Data::get<int>(*m_ptr, 2);
		ObjectGroup::tmp = 0;
		Dummy = false;
		if (*m_ptrToHeadNode == prev)
			m_node = NULL;
		else
			m_node = new Node(m_ptrToHeadNode, 1, NULL);
		print();
		prev = *m_ptrToHeadNode;
		m_count = ObjectGroup::tmp;
	}

	ObjectGroup::~ObjectGroup()
	{
		if (m_node != NULL)
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
		if (0 && m_node != NULL)
			m_node->print();
	}

	void		ObjectGroup::display()
	{
		if (m_node != NULL)
			m_node->display();
	}
}