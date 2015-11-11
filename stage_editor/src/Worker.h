#pragma once

#include "Object.h"
#include "Image.h"
#include "TestPosition.h"

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
public:
	Worker();
	~Worker();
	static void		print() { Instance->localPrint(); }
	static void		display() { Instance->localDisplay(); }
	bool		saveData();
	static void	init();

private:
	void		localPrint();
	void		localDisplay();
public:
	int						*m_header;
	static std::vector<Image *>	m_images;
	std::vector<Map::ObjectGroup *> m_testPositions;
	int						m_strOrigin;
	int						m_origin;
	static Worker			*Instance;
};
