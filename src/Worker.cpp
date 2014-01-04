#include <iostream>
#include <cstring>

#include "Node.h"
#include "Worker.h"
#include "Data.h"

using namespace Header;

int						*Worker::m_header;
std::vector<Image *>	Worker::m_images;

bool		Worker::loadData()
{
	if (Data::getHeader() == NULL)
		return false;
	m_header = reinterpret_cast<int *> (Data::getHeader());

	//Load Objects
	m_images.resize(m_header[NB_BASE]);

	int		origin = m_header[TABLE_PTR] + m_header[NB_TABLE] * 4; 
	int		strOrigin = origin + m_header[NB_BASE] * 8;

	Data::solveEndianTo(strOrigin);
	Data::m_stringPtr = strOrigin + 0x20;

	for (int i = 0; i < m_header[NB_BASE]; i++)
		m_images[i] = new Image(origin + i * 8, strOrigin);
	return true;
}

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
	//CollData::print();
}

void		Worker::display()
{
//	for (unsigned int i = 0; i < m_images.size(); i++)
//		m_images[i]->display();
	ANode::globalDisplay();
}