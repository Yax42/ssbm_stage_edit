#pragma once
#include <vector>
#include <string>

class DatFile
{
private:
	DatFile();
	~DatFile();
public:
	static void				list(const std::string &path);
	static void				set(const std::string &path);
	static std::string		current();
	static std::string		next();
	static std::string		prev();
	static void				applyNext();
	static void				applyPrev();
	static void				save();
private:
	static void				apply(const std::string &path);
	static std::vector<std::string>		_fileList;
	static int							_idx;
};
