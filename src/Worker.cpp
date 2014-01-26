#include <iostream>
#include <cstring>

#include "Node.h"
#include "TestPosition.h"
#include "Worker.h"
#include "Data.h"

#define BUGGY_OFFSET 0

using namespace Header;

int						*Worker::m_header;
std::vector<Image *>	Worker::m_images;
int						Worker::m_origin;
int						Worker::m_strOrigin;


bool	isOk(Image *img)
{
	//return !(img->isEqual("map_head") || img->isEqual("coll_data") || img->isEqual("grGroundData"));
	return (img->isEqual("map_head"));
}

bool	isOk(float v)
{
	int		*a = (int *)&v;
	return (v == 0 || ((*a > 10000 || *a < -10000) && v < 1000.0 && v > -1000 && (v > 0.1 || v < -0.1)));
}

bool	isTriositionOk(int id)
{
#if 1
		for (int j = 0; j < 3; j++)
			if (*Data::get<float>(id, j) != 0)
				return false;

		for (int j = 3; j < 6; j++)
			if (*Data::get<float>(id, j) <= 0 || *Data::get<float>(id, j) > 50)
				return false;

		return (isOk(*Data::get<float>(id, 6)) &&
			isOk(*Data::get<float>(id, 7)));
#else
		for (int j = 0; j < 9; j++)
			if (!isOk(*Data::get<float>(id, j)))
				return false;
		return true;
#endif

}

bool		Worker::loadData()
{
	if (Data::getHeader() == NULL)
		return false;
	m_header = reinterpret_cast<int *> (Data::getHeader());

	//Load Objects
	m_images.resize(m_header[NB_BASE]);

	m_origin = m_header[TABLE_PTR] + m_header[NB_TABLE] * 4; 
	m_strOrigin = m_origin + (m_header[NB_BASE] + BUGGY_OFFSET * 8) * 8;

	Data::solveEndianTo(m_strOrigin);
	Data::m_stringPtr = m_strOrigin + 0x20;

	for (int i = 0; i < m_header[NB_BASE]; i++)
		m_images[i] = new Image(m_origin + i * 8, m_strOrigin);

	for (int j = 0; j < m_header[NB_BASE]; j++)
		for (int i = j + 1; i < m_header[NB_BASE]; i++)
			if (i != j && *m_images[i]->m_deepPtr < *m_images[j]->m_deepPtr)
			{
				Image *tmp = m_images[i];
				m_images[i] = m_images[j];
				m_images[j] = tmp;
			}

	std::string name = "NONE";
	int imgCount = 0;
	int test = 0;
	bool isFirst = true;
	for (int i = 12; i < m_origin; i += 4)
	{
		if (m_images.size() > imgCount && *m_images[imgCount]->m_deepPtr < i)
		{
			name = m_images[imgCount]->m_str;
			imgCount++;
		}
		if (imgCount > 0 && !isOk(m_images[imgCount - 1]))
			continue;

		if (isTriositionOk(i))
		{
			//if (i < 100000 || i > 110000) continue; if (isFirst){ isFirst = false; continue;} //NBa
			//if (i < 60000 || i > 70000) continue; //NLa

			std::cout << test++ << "\t" << i << std::endl;
			new TestPosition(Data::get<int>(i), name, imgCount);
			i+=4;
		}
		if (i >= CollData::m_ptr[CollData::LOCATIONS])
			break;
	}
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
	//Data::print(*m_images[0]->m_deepPtr, 40);
	/*
	for (int i = 0; i < 3; i++)
		*Data::get<float>(107476, i) = 20;

	for (int i = 0; i < 3; i++)
		*Data::get<float>(107452, i) = 20;
		*/

	//Data::print(107360, 120);
	CollData::print();
}

void		Worker::display()
{
//	for (unsigned int i = 0; i < m_images.size(); i++)
//		m_images[i]->display();
	ANode::globalDisplay();
}