#include <assert.h>

#include "Window.h"
#include "Map_Group.h"
#include "Map_Node.h"
#include "MapHead.h"

MapHead						*MapHead::Instance = NULL;

void		MapHead::print()
{
	assert(m_ptr != NULL); 
	std::cout << "MapHead\t\tptr" << *m_ptr << std::endl;
	for (unsigned int i = 0; i < m_groups.size(); i++)
	{
		m_groups[i]->print();
	}
}

void		MapHead::display()
{
	assert(m_ptr != NULL); 
	for (unsigned int i = 0; i < m_groups.size(); i++)
		m_groups[i]->display();
}

MapHead::MapHead(int *ptr)
{
	m_deepPtr = ptr;
	m_ptr = Data::get<int>(*ptr);

	
	for (int *i = m_ptr; *i != 0; i += 2)
	{
		Map::ObjectGroup	*cur = new Map::ObjectGroup(&i[0], &i[1]);
		if (Map::ObjectGroup::Dummy)
			delete cur;
		else
			m_groups.push_back(cur);
	}
}

void		MapHead::init(int *ptr)
{
	if (Instance != NULL)
		delete Instance;
	Instance = new MapHead(ptr);
}

MapHead::~MapHead()
{
	for (unsigned int i = 0; i < m_groups.size(); i++)
		delete m_groups[i];
	m_groups.clear();
}