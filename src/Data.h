#pragma once

#include <vector>
#include <string>
#include <assert.h>

#include "MyMath.h"

#define Byte	unsigned char

class Data
{
private:
	Data();	
	~Data();
public:
	static bool				read(const std::string &name);
	static bool				write(const std::string &name);
	static bool				isInside(int idx) { return (idx >= 0 && idx < m_fileSize); }
	template <class T>
	static T				*get(int idx, int offset = 0)
	{
		int actualOffset = offset * sizeof(T);
		//assert(idx + actualOffset >= 0 && idx + actualOffset < m_fileSize);
		if (idx + actualOffset >= 0 && idx + actualOffset < m_fileSize)
			return (T *)(&m_data[idx + actualOffset]);
		return NULL;
	}
	static Byte				*getHeader() { return m_buffer; }
	static void				solveEndianTo(int to);
	static void				print(int idx, int count = 10, bool fourOct = false);
	static void				print(int *idx, int count = 10, bool fourOct = false);
	static void				strongPrint(int idx, int count = 5, int deepLimit = 5, int tab = 0);
	static int				getId(void *ptr) { return (((int)ptr) - ((int)m_data)); }

	static int				*m_intData;
private:
	static void				solveFileEndian();

	static Byte						*m_buffer;
	static int						*m_intBuffer;

	static Byte						*m_data;
public:
	static int						m_globalFileSize;
	static int						m_fileSize;
	static int						m_stringPtr;
	static int						Dummy[100];
};