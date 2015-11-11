#include <iostream>
#include <cstring>

#include "Node.h"
#include "TestPosition.h"
#include "Worker.h"
#include "Data.h"

#define BUGGY_OFFSET 0

using namespace Header;

Worker					*Worker::Instance = NULL;
std::vector<Image *>	Worker::m_images;

void	Worker::init()
{
	if (Instance != NULL)
		delete Instance;
	Instance = new Worker();
}

Worker::~Worker()
{

	for (unsigned int i = 0; i < m_images.size(); i++)
		delete m_images[i];
	for (unsigned int i = 0; i < m_testPositions.size(); i++)
		delete m_testPositions[i];
}

bool	isOk(Image *img)
{
	//return !(img->isEqual("map_head") || img->isEqual("coll_data") || img->isEqual("grGroundData"));
	return (img->isEqual("map_head"));
}

bool	isOk(float v)
{
	int		*a = (int *)&v;
	return ((v > 0.00000001 || v < -0.00000001) && v > -1000000 && v < 1000000) || (*a == 0);
	//return (*a == 0 || ((v > 0.001f || v < -0.001f || true) && (*a > 10000 || *a < -10000) && v < 10000.0 && v > -10000));// && (v >= 0.1 || v <= -0.1)));
}

bool	isTriositionOk(int id)
{
#if 0
	for (int j = 0; j < 3; j++)
		if (*Data::get<int>(id, j) != 0)
		//if (!isOk(*Data::get<float>(id, j)))
			return false;

	for (int j = 3; j < 6; j++)
		if (*Data::get<float>(id, j) <= 0 || *Data::get<float>(id, j) > 50 || !isOk(*Data::get<float>(id, j)))
			return false;

		return (isOk(*Data::get<float>(id, 6)) &&
			isOk(*Data::get<float>(id, 7)) && 
			isOk(*Data::get<float>(id, 8)));
#else
		for (int j = 0; j < 9; j++)
			if (!isOk(*Data::get<float>(id, j)))
				return false;
		return true;
#endif

}

Worker::Worker()
{
	if (Data::getHeader() == NULL)
		return;
	m_header = reinterpret_cast<int *> (Data::getHeader());

	//Load Objects
	m_images.resize(m_header[NB_BASE]);

	m_origin = m_header[TABLE_PTR] + m_header[NB_TABLE] * 4; 
	m_strOrigin = m_origin + (m_header[NB_BASE] + BUGGY_OFFSET * 8) * 8;

	if (m_strOrigin > Data::m_fileSize)
		throw (new std::exception("unable to to process the file"));

	Data::solveEndianTo(m_strOrigin);
	Data::m_stringPtr = m_strOrigin + 0x20;

	//for (int i = 0; i < m_header[NB_TABLE]; i++) { std::cout << i << "\t" << *Data::get<int>(m_header[TABLE_PTR] + i * 4) << std::endl; }

	for (int i = 0; i < m_header[NB_BASE]; i++)
		m_images[i] = new Image(m_origin + i * 8, m_strOrigin);

	for (int j = 0; j < m_header[NB_BASE]; j++)
		for (int i = j + 1; i < m_header[NB_BASE]; i++)
			if (i != j && *m_images[i]->m_ptr < *m_images[j]->m_ptr)
			{
				Image *tmp = m_images[i];
				m_images[i] = m_images[j];
				m_images[j] = tmp;
			}
	for (int i = 0; i < m_header[NB_BASE]; i++)
		m_images[i]->init();

	std::string name = "NONE";
	int imgCount = 0;
	int test = 0;
	bool isFirst = true;
	int imageOrigin = 0;
#if false
	for (int i = 0; i < m_origin; i += 4)
	{

		if (m_images.size() > imgCount && *m_images[imgCount]->m_ptr < i)
		{
			name = m_images[imgCount]->m_str;
			imgCount++;
			imageOrigin = i;
		}
		if (imgCount > 0 && !isOk(m_images[imgCount - 1]))
			continue;

		int origin = i -= 5 * 4;
		if (isTriositionOk(i) && origin)
		{
			m_testPositions.push_back(new TestPosition(Data::get<int>(i), name, (i - imageOrigin) / 4));//imgCount);
			i+=8*4;
		}
	}
#if false
	int count = 0;
	for (int i = 0; i < m_origin; i += 4)
		if (*Data::get<float>(i) == 5)
		{
			//int a = *Data::get<int>(i);
			*Data::get<float>(i) = 1;
			count++;
		}
#endif
#endif
#if 0
	int id = 0;
	for (int *origin = Data::get<int>(0); origin < m_images[0]->m_ptr; origin++)
	{
		if (*origin < Data::getId(m_images[0]->m_ptr)
			|| *origin > Data::m_fileSize - 300)
			continue;
		if (isTriositionOk(*origin + 5 * 4))
			m_testPositions.push_back(new Map::ObjectGroup(origin, &id));
	}
#endif
}

void		Worker::localPrint()
{
	std::cout << "File size " << m_header[Header::FILE_SIZE] << std::endl;
	std::cout << "Number of table " << m_header[Header::NB_TABLE] << std::endl;
	std::cout << "Number of base " << m_header[Header::NB_BASE] << std::endl;
	std::cout << "Number of reference " << m_header[Header::NB_REF] << std::endl;
	std::cout << "Magic number " << m_header[Header::MAGIC] << std::endl;
	std::cout << "Void1 " << m_header[Header::VOID1] << std::endl;
	std::cout << "Void2 " << m_header[Header::VOID2] << std::endl << std::endl;;

#if 1 // print sections list
	for (unsigned int i = 0; i < m_images.size(); i++)
		m_images[i]->print();
#endif

	//Data::print(*m_images[0]->m_deepPtr, 40);
	/*
	for (int i = 0; i < 3; i++)
		*Data::get<float>(107476, i) = 20;

	for (int i = 0; i < 3; i++)
		*Data::get<float>(107452, i) = 20;
		*/

	//Data::print(107360, 120);
	MapHead::Instance->print();

	//CollData::print();
	GrGroundData::Instance->print();
}

void		Worker::localDisplay()
{
//	for (unsigned int i = 0; i < m_images.size(); i++)
//		m_images[i]->display();
	ANode::globalDisplay();
}

