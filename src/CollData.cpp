#include <assert.h>

#include "Window.h"
#include "CollData.h"

int									*CollData::m_ptr = NULL;
int									*CollData::m_deepPtr = NULL;

std::vector<Coll::Node *>			CollData::m_nodes;
int									CollData::m_count = 0;
std::vector<Coll::LinkNode *>		CollData::m_links;
int									CollData::m_countLink = 0;
std::vector<Coll::ElemNode *>		CollData::m_elems;
int									CollData::m_countElem = 0;

void		CollData::print()
{
	assert(m_ptr != NULL); 
	std::cout << "CollData\t\tptr" << *m_ptr << "\tcount:" << m_count << "\tlinkCount:" << m_countLink << std::endl;
	Data::print(*m_deepPtr, 30);
	return ;
	for (unsigned int i = 0; i < m_count; i++)
		m_nodes[i]->print();
}

void		CollData::display()
{
	assert(m_ptr != NULL); 
	for (unsigned int i = 0; i < m_count; i++)
		m_nodes[i]->display();
}

void		CollData::load(int *ptr)
{
	m_deepPtr = ptr;
	m_ptr = Data::get<int>(*ptr);
	
	//NODES
	m_count = m_ptr[NUMBER];
	int		*ptrToNodes = Data::get<int>(m_ptr[LOCATIONS]);
	for (int i = 0; i < m_count; i++)
	{
		m_nodes.push_back(new Coll::Node(ptrToNodes + i * 2, i));
	}

	//LINKS
	m_countLink = m_ptr[LINK_COUNT];
	int		*ptrToLinks = Data::get<int>(m_ptr[LINKS]);
	for (int i = 0; i < m_countLink; i++)
	{
		m_links.push_back(new Coll::LinkNode(ptrToLinks + i * 4, i));
	}

	//ELEM
	m_countElem = m_ptr[ELEM_COUNT];
	int		*ptrToElems = Data::get<int>(m_ptr[ELEMS]);
	std::cout << m_countElem << " " << m_ptr[ELEMS] << std::endl;
	for (int i = 0; i < m_countElem; i++)
	{
		m_elems.push_back(new Coll::ElemNode(ptrToElems + i * 10, i));
		//m_elems[i]->print();
	}
	/*
	for (int i = 0; i < 4; i++)
	{
	std::cout << i << "\tNEXT HUGE FAT TRUC" << std::endl;
		Data::print(m_ptr[9] + i * 40, 40);
	}
	*/

}

void		CollData::clean()
{
	if (m_nodes.size() > 0)
	{
		for (unsigned int i = 0; i < m_nodes.size(); i++)
			delete m_nodes[i];
		m_nodes.clear();
	}
}