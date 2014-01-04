#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <assert.h>
#include <bitset>

#include "Object.h"
#include "Data.h"
#include "MyMath.h"

Byte			*Data::m_buffer = NULL;
int				*Data::m_intBuffer = NULL;
int				Data::m_fileSize;	//0x00
Byte			*Data::m_data;		//0x20
int				Data::m_stringPtr = -1;
int				Data::Dummy[100] = {0};

template <class T>
void	fixEndian(T &val) // to little endian
{
	static const char SwapTest[2] = { 1, 0 };
	if( *(short *) SwapTest == 1)
	{
		union
		{
			T f;
			unsigned char b[sizeof(T)];
		} dat1, dat2;

		dat1.f = val;
		for (int i = 0; i < sizeof(T); i++)
			dat2.b[i] = dat1.b[sizeof (T) - 1 - i];
		val = dat2.f;
	}
} 

bool		Data::read(const std::string &name)
{
	HANDLE hFile; 
	DWORD bytesRead = 0;
	BOOL bErrorFlag = FALSE;

	hFile = CreateFile(name.c_str(),                // name of the write
					   GENERIC_READ,          // open for reading
					   FILE_SHARE_READ,       // share for reading
					   NULL,                  // default security
					   OPEN_EXISTING,         // existing file only
					   FILE_ATTRIBUTE_NORMAL, // normal file
					   NULL);   

	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		std::cout << "Error: unable to open " << name << std::endl;
		return false;
	}

	m_stringPtr = -1;
	m_fileSize = 0;
	bErrorFlag = ReadFile(hFile, &m_fileSize, 4, &bytesRead, NULL);
	fixEndian(m_fileSize);
	if (m_fileSize > 10000000) // limit the size to 10Mo
	{
		CloseHandle(hFile);
		std::cout << "Error: file" << name << " bigger than 10Mo, can't handle it." << std::endl;
		return false;
	}
	if (m_buffer != NULL)
		delete (m_buffer);
	m_buffer = new Byte[m_fileSize];
	m_intBuffer = (int *) m_buffer;
	m_intBuffer[0] = m_fileSize;
 
	for (int totalBytesRead = 4; totalBytesRead != m_fileSize; totalBytesRead += bytesRead)
	{
		bErrorFlag = ReadFile( 
						hFile,										// open file handle
						m_buffer + totalBytesRead,					// start of data to write
						m_fileSize - totalBytesRead,				// number of bytes to write
						&bytesRead,									// number of bytes that were written
						NULL);										// no overlapped structure

		if (FALSE == bErrorFlag)
		{
			std::cout << "Error: unable to read " << name << std::endl;
			CloseHandle(hFile);
			return false;
		}
	}
	m_data = (Byte *)&m_intBuffer[8];
	for (int i = 1; i < 8; i++)
		fixEndian(m_intBuffer[i]);
	CloseHandle(hFile);
	return true;
}

bool		Data::write(const std::string &name)
{
	if (m_buffer == NULL)
	{
		std::cout << "Error: no file loaded at the first place." << std::endl;
		return (false);
	}
	HANDLE hFile; 
	DWORD bytesToWrite = (DWORD) m_fileSize;
	DWORD bytesWritten = 0;
	BOOL bErrorFlag = FALSE;


	hFile = CreateFile(name.c_str(),                // name of the write
					   GENERIC_WRITE,          // open for writing
					   0,                      // do not share
					   NULL,                   // default security
					   CREATE_ALWAYS,             // create new file only
					   FILE_ATTRIBUTE_NORMAL,  // normal file
					   NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		std::cout << "Error: unable to open " << name << std::endl;
		return false;
	}

	solveFileEndian();

	for (int totalBytesWritten = 0; totalBytesWritten != m_fileSize; totalBytesWritten += bytesWritten)
	{
		bErrorFlag = WriteFile( 
						hFile,           // open file handle
						m_buffer + totalBytesWritten,      // start of data to write
						m_fileSize - totalBytesWritten,  // number of bytes to write
						&bytesWritten, // number of bytes that were written
						NULL);            // no overlapped structure

		if (FALSE == bErrorFlag)
		{
			std::cout << "Error: unable to write on " << name << std::endl;
			CloseHandle(hFile);
			solveFileEndian();
			return false;
		}
	}
	CloseHandle(hFile);
	solveFileEndian();
	return true;
}

void		Data::solveEndianTo(int to)
{
	to = to / 4 + 8;
	for (int i = 8; i < to; i++)
		fixEndian(m_intBuffer[i]);
}

void		Data::solveFileEndian()
{
	assert(m_stringPtr != -1);

	for (int i = 0; i < m_stringPtr / 4; i++)
		fixEndian(m_intBuffer[i]);
}

void		Data::print(int idx, int count)
{
	if (idx + count > m_fileSize)
		return;
	std::cout << "Index:\t" << idx << std::endl;
	for (int i = 0; i < count; i++)
		std::cout << i << "\t" <<  Math::binStr(*get<int>(idx, i)) << " int:" << *get<int>(idx, i) << " float:" << *get<float>(idx, i) << std::endl;
}
void	printTab(int count)
{
	for (int i = 0; i < count; i++)
		std::cout << "\t";
}
 void				Data::strongPrint(int idx, int count, int tab)
 {
	std::cout << "Index:\t" << idx << std::endl;
	if (idx + count > m_fileSize)
	{
		printTab(tab);
		std::cout << "pointer out of boundaries" << std::endl;
		return ;
	}
	for (int i = 0; i < count; i++)
	{
		int		*data = get<int>(idx, i);

		printTab(tab);
		std::cout << i << ": ";
		if (Math::abs(*data) > m_fileSize)
		{
			if (Math::abs(*(float *) data) < 0.01)
			{
				std::cout << Math::binStr(*data) << std::endl;
			}
			else
			{
				std::cout  << *((float *) data);
				std::cout  << "\t" << ((float *) *data);
				std::cout << "\t" << *data << std::endl;
			}
		}
		else if (*data > 10000 && *data != 65536 && *data != 65537 && *data != 65792)
			strongPrint(*data, count, tab + 1);
		else
			std::cout << *data << std::endl;
	}
 }