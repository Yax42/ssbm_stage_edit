#pragma once

#include <iostream>
#include <string>

class DynamicVar
{
public:
	DynamicVar(const std::string &name, int &val, bool &inc, bool &dec) : m_name(name), m_val(&val), m_inc(&inc), m_dec(&dec) {}
	DynamicVar()
	{
		m_val = NULL;
	}
	~DynamicVar(){}
	void		proc()
	{
		if (m_val == NULL)
			return;
		float factor =	  (*m_alt && *m_ctrl) ? 1000000
						: (*m_alt && *m_shift) ? 100000
						: (*m_ctrl && *m_shift) ? 10000
						: *m_alt ? 1000
						: *m_ctrl ? 100
						: *m_shift ? 10
						: 1;
		*m_val += (*m_inc - *m_dec) * factor;
		if (*m_inc || *m_dec)
			print();
	}

	void		print()
	{
		if (m_val == NULL)
			return;
		std::cout << m_name << ":\t" << *m_val << std::endl;
	}

private:
	std::string		m_name;
	int				*m_val;
	bool			*m_inc;
	bool			*m_dec;
	static bool		m_dummy;
public:
	static bool		*m_shift;
	static bool		*m_ctrl;
	static bool		*m_alt;

};

