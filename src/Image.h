#pragma once

#include <string>
#include "Object.h"
#include "MapHead.h"

class Image: public Object
{
public:
	Image(int pos, int strPos, int deep = 0) : Object(deep)
	{
		m_ptr = Data::get<int>(pos);
		m_strOffset = Data::get<int>(pos, 1);
		m_str = Data::get<char>(strPos, *m_strOffset);
		if (isEqual("map_head"))
			MapHead::load(Data::get<int>(*m_ptr));
	}
	~Image(){}
	virtual void	print()
	{
		printTab();
		std::cout << *m_strOffset << "\t " << std::string(m_str) << std::endl;
		if (isEqual("map_head"))
			MapHead::print();
	}

	virtual void	display()
	{
		if (isEqual("map_head"))
			MapHead::display();
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