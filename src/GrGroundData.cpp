#include <assert.h>

#include "Data.h"
#include "GrGroundData.h"
#include "Window.h"

int									*GrGroundData::m_ptr = NULL;
int									*GrGroundData::m_deepPtr = NULL;

void		GrGroundData::print()
{
	assert(m_ptr != NULL); 
	std::cout << "GrGroundData\t\tptr" << *m_ptr << std::endl;
	Data::print(m_ptr, GrGroundData::COUNT);
}

void		GrGroundData::display()
{
	assert(m_ptr != NULL); 
}

void		GrGroundData::load(int *ptr)
{
	m_deepPtr = ptr;
	m_ptr = Data::get<int>(*ptr);
}

void		GrGroundData::clean()
{
}