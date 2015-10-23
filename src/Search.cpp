#include "Search.h"
#include "Data.h"
#include "Image.h"
#include "Worker.h"


Search::Search()
{
	Found = false;
}

Search::Search(int *ptr)
{
	int target = Data::getId(ptr);
	m_origin = ptr;
	m_i = -1;
	int *data = Data::m_intData;
	int imgId = 1;
	std::string imgName = "aaaaaa";
	for (int i = 0; i < Data::m_fileSize / 4; i++)
	{
		if (imgId < Worker::m_images.size() && &data[i] == Worker::m_images[imgId]->m_ptr)
		{
			imgName = Worker::m_images[imgId]->m_str;
			imgId++;
		}
		if (0 == imgName.compare(imgName.length() - 5, 5, "image"))
			continue;
		if (data[i] == target)
			m_founds.push_back(&data[i]);
	}
	Count = m_founds.size();
	Found = (Count > 0);
}
Search::~Search() {};

int					*Search::next()
{
	if (!Found)
		return NULL;
	m_i = (m_i + 1) % m_founds.size();
	return m_founds[m_i];
}

int					*Search::prev()
{
	if (!Found)
		return NULL;
	int size = m_founds.size();
	m_i = (m_i - 1 + size) % size;
	return m_founds[m_i];
}
