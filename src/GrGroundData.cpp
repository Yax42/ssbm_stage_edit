#include <assert.h>

#include "Data.h"
#include "GrGroundData.h"
#include "Window.h"

GrGroundData	*GrGroundData::Instance = NULL;

const std::vector<std::string> GrGroundData::m_labels =
{
	"Entire stage size",
	"",
	"",
	"",
	"",
	"?Minimum tilt & tilt scaling",
	"?Horizontal rotation factor",
	"?Vertical rotation factor",
	"Fixed-ness of camera",
	"Character bubble buffer multiplier",
	"Camera speed smoothness",
	"?Unknown 1",
	"?Pause minimum Z position",
	"?Pause initial Z position",
	"?Pause max Z position",
	"",
	"?Pause max angle up",
	"?Pause max angle left",
	"?Pause max angle right",
	"?Pause max angle down",
	"?Unknown 3",
	"?Unknown 4",
	"?Unknown 5",
	"?Unknown 6",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Pointer to ? table",
	"Nb entries in ? table",
	"bble col TL",
	"bble col TM",
	"bble col TR",
	"bble col ST",
	"bble col SM",
	"bble col SB",
	"bble col BL",
	"bble col BM",
	"bble col BR",
};
GrGroundData::GrGroundData(int *ptr) : Ptr(ptr, m_labels, "GrGroundData", 0, false)
{
	m_floatPtr = (float *)m_ptr;
}

GrGroundData::~GrGroundData()
{

}

void		GrGroundData::init(int *ptr)
{
	Instance = new GrGroundData(ptr);
}

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

void		GrGroundData::act()
{
	if (g_window.m_tmp[0] == -1)
		print();
	else if ((g_window.m_tmp[0] <= INT3 && g_window.m_tmp[0] >= INT1) || g_window.m_tmp[0] == INT4)
		m_ptr[g_window.m_tmp[0]] = g_window.m_tmp[1];
	
	else if (g_window.m_tmp[0] >= 0 && g_window.m_tmp[0] <= INT4)
		m_floatPtr[g_window.m_tmp[0]] = g_window.m_tmp[1] / 10.0;
}