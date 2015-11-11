#include <assert.h>

#include "Window.h"
#include "Map_Group.h"
#include "Map_Node.h"
#include "MapHead.h"
#include "Worker.h"

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

	
#if 0
	for (int *i = m_ptr; *i != 0; i += 2)
	{
		Map::ObjectGroup	*cur = new Map::ObjectGroup(&i[0], &i[1]);
#if 0
		if (Map::ObjectGroup::Dummy)
			delete cur;
		else
#endif
			m_groups.push_back(cur);
	}
#else
	int id = 0;
	for (int *origin = Data::get<int>(0); origin < Data::get<int>(*Worker::m_images[0]->m_ptr); origin++)
	{
		int v = Data::getId(origin);
		if (*origin < *Worker::m_images[0]->m_ptr
			|| *origin > Data::m_fileSize - 300)
			continue;
		int *data = Data::get<int>(*origin);
		float *fdata = Data::get<float>(*origin);

		bool a = Data::isTransformOk(*origin + 5 * 4);
		bool b = Data::isInside(data[Map::Node::DATA_PTR]);
		bool c = Data::isInside(data[Map::Node::CHILD_PTR]);
		bool d = Data::isInside(data[Map::Node::NEXT_PTR]);
		bool e = data[Map::Node::EMPTY1] == 0;
		bool f = data[Map::Node::EMPTY2] == 0;

		if (Data::isTransformOk(*origin + 5 * 4)
			&& Data::isInside(data[Map::Node::DATA_PTR])
			&& Data::isInside(data[Map::Node::CHILD_PTR])
			&& Data::isInside(data[Map::Node::NEXT_PTR])
			&& data[Map::Node::EMPTY1] == 0
			&& data[Map::Node::EMPTY2] == 0)
		{
			m_groups.push_back(new Map::ObjectGroup(origin, &id, 0, true));
			id++;
		}
	}
#endif
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