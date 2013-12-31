#include <assert.h>

#include "Window.h"
#include "MapHead.h"

int									*MapHead::m_ptr = NULL;
std::vector<Map::ObjectGroup *>		MapHead::m_groups;

void		MapHead::print()
{
	assert(m_ptr != NULL); 
	std::cout << "MapHead\t\tptr" << *m_ptr << std::endl;
	for (unsigned int i = 0; i < m_groups.size(); i++)
		m_groups[i]->print();
}

void		MapHead::display()
{
	assert(m_ptr != NULL); 
	for (unsigned int i = 0; i < m_groups.size(); i++)
		m_groups[i]->display();
}

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

Map::Node	*MapHead::getMouseTarget()
{
	Map::Node	*result;
	for (unsigned int i = 0; i < m_groups.size(); i++)
	{
		result = m_groups[i]->getMouseTarget();
		if (result != NULL)
			return result;
	}
	return NULL;
}