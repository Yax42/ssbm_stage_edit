#include <iostream>
#include <cstring>

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

	for (int i = 0; i < m_header[NB_BASE]; i++)
		m_images[i] = new Image(origin + i * 8, strOrigin);
	return true;
}
