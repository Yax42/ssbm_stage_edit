#include"SFML\Graphics.hpp"
#include "Data.h"
#include "Worker.h"
#include "Window.h"
#include "Resources.h"
#include "BytesMatrix.h"


int		main(int ac, char *av)
{
	Window::init();
	Resources::init();
	Data::read("../data/test.dat");
	Worker::loadData();
	BytesMatrix::Instance = new BytesMatrix(300.f, 128.f);

	Worker::print();
	g_window.loop();
	//system("pause");
	return (0);
}
