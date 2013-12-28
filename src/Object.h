#pragma once

#include <string>
#include <iostream>

#include "Data.h"

class Object
{
public:
	Object(int deep = 0) {m_deep = deep; }
	virtual ~Object() {}
	//template <class T>
	//T		*data(int offset = 0) { return Data::get<T>(m_position + offset);}
	virtual void		print() = 0;
protected:
	void	printTab()
	{
		for (int i = 0; i < m_deep; i++)
			std::cout << "\t";
	}

	int			m_deep;
};