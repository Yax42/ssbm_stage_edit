#pragma once
#include <vector>


class Search
{
public:
	Search();
	Search(int *ptr);
	~Search();

	int					*next();
	int					*prev();
	bool					Found;
	int						Count;
	int						m_i;
	std::vector<int *>		m_founds;
	int						*m_origin;
};