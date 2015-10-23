#pragma once
#include <fstream>
#include <vector>

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "Data.h"
#include "Worker.h"
#include "WriteDataInFile.h"

void		WriteDataInFile::Do()
{

#if 1
	std::vector<std::string> types =
	{
		"itemdata", //ptr
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
	};
#elif 0
	std::vector<std::string> types =
	{
		"itemdata", //ptr
		"byte",
		"ptr",
		"float",
		"float",
		"float",
		"float",
		"ptr",
		"float",
		"float",
		"float",
		"float",
		"float",
		"float",
		"float",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"ptr",
		"byte",
		"ptr",
		"ptr",
		"ptr",
		"byte",
		"byte",
		"byte",
		"byte",
		"ptr",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"byte",
		"ptr",
		"ptr",
	};
#elif 0
	std::vector<std::string> types =
	{
		"grGroundParam",
		"float",
		"int2",
		"int2",
		"int4",
		"int4",
		"int4",
		"float",
		"float",
		"float",
		"float",
		"float",
		"int4",
		"int4",
		"int4",
		"int4",
		"float",
		"float",
		"float",
		"float",
		"float",
		"float",
		"float",
		"float",
		"float",
		"float",
		"float",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"int2",
		"ptr",
		"int4",
	};
#elif 0
std::vector<std::string> types =
{
	"coll_data",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
	"int2",
};
#elif 1
std::vector<std::string> types =
{
	"yakumono_param",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",
	"byte",

};
#endif

	std::vector<std::string> lines = types;
	std::vector<std::string> fileList;

		std::string dir = "testData//";
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFile(std::string(dir + "*.dat").c_str(), &FindFileData);
		bool fileOver = (hFind != INVALID_HANDLE_VALUE);
		while (fileOver)
		{
			fileList.push_back(std::string(FindFileData.cFileName));
			fileOver = FindNextFile(hFind, &FindFileData);
		}
		FindClose(hFind);
	}
	for (int i = 0; i < fileList.size(); i++)
	{
		std::cout << i << "/" << fileList.size() << " " << fileList[i] << std::endl;
		bool exep = false;
		int *ptr = NULL;
		int imgIdx = 0;
		try
		{
			Data::read(dir + fileList[i]);
			Worker::loadData();
			Image *image = NULL;
			for (int i = 0; i < Worker::m_images.size(); i++)
				if (Worker::m_images[i]->isEqual(types[0]))
				{
					image = Worker::m_images[i];
					imgIdx = i;
					break;
				}
			ptr = Data::get<int>(*image->m_ptr);
			//ptr = Data::get<int>(*ptr);

		}
		catch (...)
		{
			std::cout << "Exception" << std::endl;
			exep = true;
		}
			int *limit = Data::get<int>(*(Worker::m_images[imgIdx + 1]->m_ptr));
			lines[0] += "," + fileList[i];
			for (int j = 0; j < types.size() - 1; j++)
			{
				std::string str = ",";
				if (limit > (&ptr[j]) && !exep)
				{
					if (ptr[j] == -1)
						str += "#FFFF";
					else if (ptr[j] == 0)
						str += "0";
					else if (types[j + 1] == "ptr")
						;
					else if (types[j + 1] == "float")
						str += std::to_string(*((float *)&ptr[j]));
					else if (types[j + 1] == "int4")
						str += std::to_string(ptr[j]);
					else if (types[j + 1] == "int2")
					{
						short *sptr = (short*)&ptr[j];
						str += std::to_string(sptr[1]) + " | " + std::to_string(sptr[0]);
					}
					else if (types[j + 1] == "byte")
					{
						char *bptr = (char*)&ptr[j];
						str += std::to_string(bptr[3]) + " | " + std::to_string(bptr[2]) + " | " + std::to_string(bptr[1]) + " | " + std::to_string(bptr[0]);
					}
				}
				lines[j + 1] += str;
			}
	}


	std::ofstream outfile("data.txt", std::ofstream::out);

	for (int j = 0; j < types.size(); j++)
	{
		lines[j] += "\n";
		outfile.write(lines[j].c_str(), lines[j].size());
	}
	outfile.close();
}