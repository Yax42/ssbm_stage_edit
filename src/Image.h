#pragma once

#include <string>
#include "Object.h"
#include "MapHead.h"
#include "CollData.h"

class Image: public Object
{
public:
	Image(int pos, int strPos, int deep = 0) : Object(deep)
	{
		m_ptr = Data::get<int>(pos);
		m_strOffset = Data::get<int>(pos, 1);
		m_str = Data::get<char>(strPos, *m_strOffset);
		if (isEqual("map_head"))
			MapHead::load(m_ptr);
		if (isEqual("coll_data"))
			CollData::load(m_ptr);
	}
	~Image(){}
	virtual void	print()
	{
		printTab();
		std::cout << *m_strOffset << "\t " << std::string(m_str) << std::endl;
		if (isEqual("itemdata"))
			Data::strongPrint(*m_ptr, 10);
		return;
		if (isEqual("map_head"))
			MapHead::print();
		if (isEqual("coll_data"))
			CollData::print();
	}

	virtual void	display()
	{
		if (isEqual("map_head"))
			MapHead::display();
		if (isEqual("coll_data"))
			CollData::display();
	}

public:
	int		*m_strOffset;
	char	*m_str;
	int		*m_ptr;
private:
	bool	isEqual(char *str)
	{
		return (std::string(m_str).compare(str) == 0);
	}
};