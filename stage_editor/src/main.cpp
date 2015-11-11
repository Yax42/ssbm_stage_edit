#include <fstream>
#include"SFML\Graphics.hpp"
#include "Data.h"
#include "Worker.h"
#include "Window.h"
#include "Resources.h"
#include "BytesMatrix.h"
#include "WriteDataInFile.h"
#include "DatFile.h"


int		main(int ac, char **av)
{
#if 1
	Window::init(1024, 768);
	Resources::init();

#if 0
	DatFile::list("../../data/list/");
#else
	DatFile::set("../../data/list/GrSh.dat");
#endif

	Worker::print();
	g_window.loop();
	//system("pause");
	return (0);
#elif 1
	WriteDataInFile::Do();
#else
	try
	{
	if (ac != 2 && ac != 4)
	{

		std::cout << "error: correct usage is \'msr <file_name_from> <file_name_to> <scale>\'" << std::endl;
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

		if (ac == 2)
		{
			Data::read(fileName);
			Worker::loadData();
			std::cout << GrGroundData::Instance->m_floatPtr[0] << std::endl;
		}
		else if (ac == 4)
		{
			float scale;
			try
			{
				scale = std::stof(av[3]);
			}
			catch (...)
			{
				std::cout << "error: \"" << av[3] << "\" is not in a good scale format (3.4, 0.01 and 99 are)" << std::endl;
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

			float previousScale = GrGroundData::Instance->m_floatPtr[0];
			GrGroundData::Instance->m_floatPtr[0] = scale;
			GrGroundData::Instance->m_ptr[3] = scale * GrGroundData::Instance->m_ptr[4] / previousScale;
			GrGroundData::Instance->m_ptr[4] = scale * GrGroundData::Instance->m_ptr[4] / previousScale;
			if (!Data::write(std::string(av[2])))
			{
				std::cout << "error: something went wrong and the file couldn't be saved correctly" << std::endl;
				return (0);
			}
		}
	}
	catch (const std::exception &exc)
	{
		std::cout << "error: " << exc.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "error: " << "unable to process the file" << std::endl;
	}
	return (0);
#endif
}
