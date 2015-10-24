#pragma once

#include <string>
#include <iostream>

#include "Data.h"
#include "Ptr.h"

class Object : public Ptr
{
public:
	Object(int *ptr, const std::vector<std::string> &labels, const std::string &name, int id, int deep, bool ptrIsOk)
		: Ptr(ptr, labels, name, id, ptrIsOk) {m_deep = deep; }
	virtual ~Object() {}
	//template <class T>
	//T		*data(int offset = 0) { return Data::get<T>(m_position + offset);}
	virtual void		print() = 0;
	virtual void		display() = 0;
protected:
	void	printTab()
	{
		for (int i = 0; i < m_deep; i++)
			std::cout << "  ";
	}

	int			m_deep;
};