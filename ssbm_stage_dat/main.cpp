#include "Data.h"
#include "Worker.h"

int		main(int ac, char *av)
{
	Data::read("test.dat");
	Worker::loadData();
	Worker::print();
	system("pause");
	return (0);
}
