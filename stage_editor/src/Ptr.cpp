#include "Ptr.h"
#include "Data.h"

std::vector<Ptr *> Ptr::List;

Ptr::Ptr(int *ptr, const std::vector<std::string> &labels, const std::string &name, int id, bool ptrIsok)
	: m_ptr(ptr), Labels(labels), Name(name), m_id(id)
{
	if (!ptrIsok)
		m_ptr = Data::get<int>(*m_ptr);

	bool done = false;
	for (auto it = List.begin(); it != List.end(); ++it)
	{
		if (m_ptr < (*it)->m_ptr)
		{
			List.insert(it, this);
			done = true;
			break;
		}
	}
	if (!done)
		List.push_back(this);
}

Ptr::~Ptr()
{
	for (auto it = List.begin(); it != List.end(); ++it)
		if (this == (*it))
		{
			List.erase(it);
			break;
		}
}
