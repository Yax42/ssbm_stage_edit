#include <assert.h>

#include "Worker.h"
#include "MapHead.h"

void		Worker::print()
{
	std::cout << "File size " << m_header[Header::FILE_SIZE] << std::endl;
	std::cout << "Number of table " << m_header[Header::NB_TABLE] << std::endl;
	std::cout << "Number of base " << m_header[Header::NB_BASE] << std::endl;
	std::cout << "Number of reference " << m_header[Header::NB_REF] << std::endl;
	std::cout << "Magic number " << m_header[Header::MAGIC] << std::endl;
	std::cout << "Void1 " << m_header[Header::VOID1] << std::endl;
	std::cout << "Void2 " << m_header[Header::VOID2] << std::endl << std::endl;;

	for (unsigned int i = 0; i < m_images.size(); i++)
		m_images[i]->print();
}

void		MapHead::print()
{
	assert(m_ptr != NULL); 
	std::cout << "MapHead\t\tptr" << *m_ptr << std::endl;
	for (unsigned int i = 0; i < m_groups.size(); i++)
		m_groups[i]->print();
}

namespace Map
{
	void		ObjectGroup::print()
	{
		printTab();
		std::cout << "ObjectGroup\tid:" << *m_id << " ptr:" << *m_ptr << " node:" << *m_ptrToHeadNode << " uk1:" << *m_unknown1 << " uk2:" << *m_unknown2 << std::endl;
		m_node->print();
	}

	void		Node::print()
	{
		std::cout.precision(4);
		printTab();
		std::cout << "Node Z" << m_intData[EMPTY1]
				<< " Flags:" << m_intData[FLAGS]
				<< " Child:" << m_intData[CHILD_PTR]
				<< " Next:" << m_intData[NEXT_PTR]
				<< " Data:" << m_intData[DATA_PTR]
				<< " Inverse:" << m_intData[INVERSE]
				<< " Z" << m_intData[EMPTY2] << std::endl;
		printTab();
		std::cout << "\tRotate:\t\tX" << m_floatData[ROTATEX] << "\tY" << m_floatData[ROTATEY] << "\tZ" << m_floatData[ROTATEZ] << std::endl;
		printTab();
		std::cout << "\tSize:\t\tX" << m_floatData[SIZEX] << "\tY" << m_floatData[SIZEY] << "\tZ" << m_floatData[SIZEZ] << std::endl;
		printTab();
		std::cout << "\tPosition:\tX" << m_floatData[LOCATIONX] << "\tY" << m_floatData[LOCATIONY] << "\tZ" << m_floatData[LOCATIONZ] << std::endl;
		if (m_child != NULL)
			m_child->print();
		if (m_next != NULL)
			m_next->print();
	}
}