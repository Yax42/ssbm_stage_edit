#include "Map_Group.h"

namespace Map
{

	const std::vector<std::string> ObjectGroup::m_labels =
	{
		"Ptr to head node",
		"Unk1",
		"Unk2",
	};
	bool	ObjectGroup::Dummy;

	ObjectGroup::ObjectGroup(int *ptr, int *id, int deep)
		: Object(ptr, m_labels , "ObjectGroup", *id, deep, false)
	{
		static int		prev = 0;
		m_ptrToHeadNode = &m_ptr[0];
		m_unknown1 = &m_ptr[1];
		m_unknown2 = &m_ptr[2];
		Dummy = false;
		if (*m_ptrToHeadNode == prev)
			m_node = NULL;
		else
		{
			m_node = new Node(m_ptrToHeadNode, 1, NULL, *this, 0);
			m_count = m_node->m_count;
		}
		//print();
		prev = *m_ptrToHeadNode;
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
			"ObjectGroup\tid:" << m_id <<
			//" ptr:" << *m_ptr <<
			//" node:" << *m_ptrToHeadNode <<
			//" uk1:" << *m_unknown1 <<
			//" uk2:" << *m_unknown2 <<
			" count:" << m_count << std::endl;
		if (m_node != NULL)
			m_node->print();
	}

	void		ObjectGroup::display()
	{
		if (m_node != NULL)
			m_node->display();
	}
}