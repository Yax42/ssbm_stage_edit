#include "DatFile.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "Data.h"
#include "Worker.h"
#include "BytesMatrix.h"
#include "Window.h"

std::vector<std::string>		DatFile::_fileList;
int								DatFile::_idx;

void			DatFile::list(const std::string &path)
{
	_fileList.clear();
	_idx = -1;
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFile(std::string(path + "*.dat").c_str(), &FindFileData);
		bool fileOver = (hFind != INVALID_HANDLE_VALUE);
		while (fileOver)
		{
			_fileList.push_back(path + std::string(FindFileData.cFileName));
			fileOver = FindNextFile(hFind, &FindFileData);
		}
		FindClose(hFind);
	}
	applyNext();
}

void				DatFile::set(const std::string &path)
{
	_fileList.clear();
	_fileList.push_back(path);
	_idx = -1;
	applyNext();
}

std::string		DatFile::current()
{
	return _fileList[_idx];
}

std::string		DatFile::next()
{
	_idx = (_idx + 1) % _fileList.size();
	return _fileList[_idx];
}

std::string		DatFile::prev()
{
	_idx = (_idx - 1 + _fileList.size()) % _fileList.size();
	return _fileList[_idx];
}
void				DatFile::applyNext()
{
	apply(next());
}
void				DatFile::applyPrev()
{
	apply(prev());
}

void				DatFile::apply(const std::string &file)
{
	ANode::clearList();
	Data::read(file);
	Worker::init();
	BytesMatrix::init(0, 64.f, Search());

	Window::Instance->m_window.setTitle(file);
}

void				DatFile::save()
{
	if (Data::write(current()))
		std::cout << "saved" << std::endl;
	else
		std::cout << "failed save" << std::endl;
}