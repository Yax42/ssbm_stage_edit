#include <assert.h>

#include "Data.h"
#include "GrGroundData.h"
#include "Window.h"

int									*GrGroundData::m_ptr = NULL;
float								*GrGroundData::m_floatPtr = NULL;
int									*GrGroundData::m_deepPtr = NULL;

void		GrGroundData::print()
{
	assert(m_ptr != NULL); 
	std::cout << "GrGroundData\t\tptr" << *m_ptr << std::endl;
	Data::print(m_ptr, GrGroundData::INT4 + 1);
}

void		GrGroundData::display()
{
	assert(m_ptr != NULL); 
}

void		GrGroundData::load(int *ptr)
{
	m_deepPtr = ptr;
	m_ptr = Data::get<int>(*ptr);
	m_floatPtr = (float *) m_ptr;
}

void		GrGroundData::clean()
{
}

void		GrGroundData::act()
{
	if (g_window.m_tmp[0] == -1)
		print();
	else if ((g_window.m_tmp[0] <= INT3 && g_window.m_tmp[0] >= INT1) || g_window.m_tmp[0] == INT4)
		m_ptr[g_window.m_tmp[0]] = g_window.m_tmp[1];
	
	else if (g_window.m_tmp[0] >= 0 && g_window.m_tmp[0] <= INT4)
		m_floatPtr[g_window.m_tmp[0]] = g_window.m_tmp[1] / 10.0;
}