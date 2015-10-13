#include"SFML\Graphics.hpp"
#include "Data.h"
#include "Worker.h"
#include "Window.h"


int		main(int ac, char *av)
{

	Data::read("../data/test.dat");
	Worker::loadData();
	Worker::print();
	Window::init();
	g_window.loop();
	//system("pause");
	return (0);
}
