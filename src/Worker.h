#pragma once

#include "Object.h"
#include "Image.h"

namespace Header
{
	enum
	{
		FILE_SIZE,
		TABLE_PTR,
		NB_TABLE,
		NB_BASE,
		NB_REF,
		MAGIC,
		VOID1,
		VOID2,
		COUNT
	};
};

class Worker
{
private:
	Worker(int deep);
	~Worker();
public:
	static void		print();
	static void		display();
	static bool		loadData();
	static bool		saveData();

public:
	static int						*m_header;
	static std::vector<Image *>		m_images;
};