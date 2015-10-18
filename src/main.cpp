#include <fstream>
#include"SFML\Graphics.hpp"
#include "Data.h"
#include "Worker.h"
#include "Window.h"
#include "Resources.h"
#include "BytesMatrix.h"


int		main(int ac, char **av)
{
#if 0
	Window::init();
	Resources::init();
	Data::read("../data/test.dat");
	Worker::loadData();
	BytesMatrix::Instance = new BytesMatrix(300.f, 128.f);

	Worker::print();
	g_window.loop();
	//system("pause");
	return (0);
#else
	if (ac != 3)
	{
		std::cout << "error: correct usage is \'msr <file_name> <scale>\'" << std::endl;
		return 0;
	}
	std::string fileName = av[1];
	//std::string fileName = "test.dat";
	std::ifstream file(fileName);
	if (!file.good())
	{
		std::cout << "error: unable to access " << fileName << std::endl;
		return 0;
	}

	float scale;
	try
	{
		scale = std::stof(av[2]);
	}
	catch (...)
	{
		std::cout << "error: \"" << av[2] << "\" is not in a good scale format (3.4, 0.01 and 99 are)" << std::endl;
		return 0;
	}
	scale = ((int)(scale * 100)) / 100.f;
	if (scale < 0.01f || scale > 100.f)
	{
		std::cout << "error: scale must be in the range of [0.01, 100.00] and right now it's " << std::to_string(scale) << std::endl;
		return 0;
	}
	Data::read(fileName);
	Worker::loadData();
	GrGroundData::Instance->m_floatPtr[0] = scale;
	GrGroundData::Instance->m_ptr[3] = scale * 800;
	GrGroundData::Instance->m_ptr[4] = scale * 1100;
	if (!Data::write("out.dat"))
	{
		std::cout << "error: something went wrong and the file couldn't be saved correctly" << std::endl;
		return (0);
	}
	std::cout << "success: your stage has been rescaled to " << std::to_string(scale) << " and saved into out.dat" << std::endl;


	return (0);
#endif
}
