#pragma once

#include <vector>
#include <string>

#define Byte	unsigned char

class Data
{
private:
	Data();	
	~Data();
public:
	static bool				read(const std::string &name);
	static bool				write(const std::string &name);
	template <class T>
	static T				*get(int idx, int offset = 0){ return (T *)(&m_data[idx + offset * sizeof(T)]); }
	static Byte				*getHeader() { return m_buffer; }
	static void				solveEndianTo(int to);

private:
	static Byte						*m_buffer;
	static int						*m_intBuffer;

	static int						m_fileSize;
	static Byte						*m_data;
};