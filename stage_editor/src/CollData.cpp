#include <assert.h>

#include "Window.h"
#include "CollData.h"

CollData	*CollData::Instance = NULL;

const std::vector<std::string> CollData::m_labels =
{
	"Spots",
	"Spots_count",
	"Links",
	"Links_count",
	"Ref_top",
	"Ref_bot",
	"Ref_right",
	"Ref_left",
	"M2",
	"Areas",
	"Area_count",
	"Mistery",
};

CollData::CollData(int *ptr) : Ptr(ptr, m_labels, "CollData", 0, false)
{
}

void CollData::init(int *ptr)
{
	if (Instance != NULL)
		delete Instance;
	Instance = new CollData(ptr);
	Instance->load();
}

void CollData::load()
{
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

	//AREAS
	m_countArea = m_ptr[ELEM_COUNT];
	int		*ptrToAreas = Data::get<int>(m_ptr[ELEMS]);
	//std::cout << m_countArea << " " << m_ptr[ELEMS] << std::endl;
	for (int i = 0; i < m_countArea; i++)
	{
		m_areas.push_back(new Coll::AreaNode(ptrToAreas + i * 10, i));
		//m_areas[i]->print();
	}

	/*
	for (int i = 0;  i < 5; i++)
	{
		std::cout << i << "\tNEXT HUGE FAT TRUC" << std::endl;
			Data::print(m_ptr[9] + 10 * m_ptr[ELEM_COUNT] + i * 30, 30);
	}
	*/

}

CollData::~CollData()
{
	for (unsigned int i = 0; i < m_nodes.size(); i++)
		delete m_nodes[i];
	for (unsigned int i = 0; i < m_links.size(); i++)
		delete m_links[i];
	for (unsigned int i = 0; i < m_areas.size(); i++)
		delete m_areas[i];
}

void		CollData::print()
{
	assert(m_ptr != NULL); 
	std::cout << "CollData\tptr " << Data::getId(m_ptr) << "\tNodePtr:" << m_ptr[LOCATIONS] << "\tLinkPtr:" << m_ptr[LINKS] << "\tAreaPtr:" << m_ptr[ELEMS] << std::endl;
	std::cout << "M2:\t" << Math::toStr<short>(m_ptr[M2], 0) << "\t" << Math::toStr<short>(m_ptr[M2], 1) << std::endl;
	std::cout << "Myste:\t" << Math::toStr<short>(m_ptr[MISTERY], 0) << "\t" << Math::toStr<short>(m_ptr[MISTERY], 1) << std::endl;
	return ;
	std::cout << "CollData\tptr " << ""  << "\tNodeCount:" << m_count << "\tLinkCount:" << m_countLink << "\tAreaCount:" << m_countArea << std::endl;
	std::cout << "Top:\t" << Math::toStr<short>(m_ptr[REF_TOP], 0) << "\t" << Math::toStr<short>(m_ptr[REF_TOP], 1) << std::endl;
	std::cout << "Bot:\t" << Math::toStr<short>(m_ptr[REF_BOT], 0) << "\t" << Math::toStr<short>(m_ptr[REF_BOT], 1) << std::endl;
	std::cout << "Right:\t" << Math::toStr<short>(m_ptr[REF_RIGHT], 0) << "\t" << Math::toStr<short>(m_ptr[REF_RIGHT], 1) << std::endl;
	std::cout << "Left:\t" << Math::toStr<short>(m_ptr[REF_LEFT], 0) << "\t" << Math::toStr<short>(m_ptr[REF_LEFT], 1) << std::endl;
	std::cout << "M2:\t" << Math::toStr<short>(m_ptr[M2], 0) << "\t" << Math::toStr<short>(m_ptr[M2], 1) << std::endl;
	std::cout << "Myste:\t" << Math::toStr<short>(m_ptr[MISTERY], 0) << "\t" << Math::toStr<short>(m_ptr[MISTERY], 1) << std::endl;
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


void			CollData::autoResolve()
{
	int			count[4] = {0, 0, 0, 0};
	int			min[4] = {1000, 1000, 1000, 1000};
	
	std::cout << Math::toStr<short>(m_ptr[4], 0) << "\t" << Math::toStr<short>(m_ptr[4], 1) << std::endl;
	std::cout << Math::toStr<short>(m_ptr[5], 0) << "\t" << Math::toStr<short>(m_ptr[5], 1) << std::endl;
	std::cout << Math::toStr<short>(m_ptr[6], 0) << "\t" << Math::toStr<short>(m_ptr[6], 1) << std::endl;
	std::cout << Math::toStr<short>(m_ptr[7], 0) << "\t" << Math::toStr<short>(m_ptr[7], 1) << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < m_countArea; i++)
		{
			count[j] += m_areas[i]->m_shortPtr[j * 2];
			if (m_areas[i]->m_shortPtr[j * 2]
					&& m_areas[i]->m_shortPtr[j * 2 + 1] < min[j])
				min[j] = m_areas[i]->m_shortPtr[j * 2 + 1];
		}
		if (min[j] < 1000)
			((short *)m_ptr)[9 + j * 2] = min[j];
		((short *)m_ptr)[8 + j * 2] = count[j];

	}

	//m_ptr[MISTERY] = 14;

	std::cout << Math::toStr<short>(m_ptr[4], 0) << "\t" << Math::toStr<short>(m_ptr[4], 1) << std::endl;
	std::cout << Math::toStr<short>(m_ptr[5], 0) << "\t" << Math::toStr<short>(m_ptr[5], 1) << std::endl;
	std::cout << Math::toStr<short>(m_ptr[6], 0) << "\t" << Math::toStr<short>(m_ptr[6], 1) << std::endl;
	std::cout << Math::toStr<short>(m_ptr[7], 0) << "\t" << Math::toStr<short>(m_ptr[7], 1) << std::endl;
}