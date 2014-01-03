#include <assert.h>

#include "Window.h"
#include "CollData.h"

int									*CollData::m_ptr = NULL;
int									*CollData::m_deepPtr = NULL;
std::vector<Coll::Node *>			CollData::m_nodes;
int									CollData::m_count = 0;

void		CollData::print()
{
	assert(m_ptr != NULL); 
	std::cout << "CollData\t\tptr" << *m_ptr << std::endl;
	//Data::strongPrint(*m_deepPtr, 30);
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
	
	m_count = m_ptr[NUMBER];
	int		*ptrToNodes = Data::get<int>(m_ptr[LOCATIONS]);

	for (int i = 0; i < m_count; i++)
	{
		m_nodes.push_back(new Coll::Node(&ptrToNodes[i * 2]));
	}
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