#pragma once
#include <set>
#include <vector>

class Ptr
{
public:
	static std::vector<Ptr *>				List;

public:
	Ptr(int *ptr, const std::vector<std::string> &labels, const std::string &name, int id, bool ptrIsok);
	~Ptr();
public:
	int										*m_ptr;
	const std::vector<std::string>			&Labels;
	const std::string						Name;
	int										m_id;
};
