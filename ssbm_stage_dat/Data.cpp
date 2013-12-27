#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#include "Object.h"
#include "Data.h"

Byte			*Data::m_buffer = NULL;
int				*Data::m_intBuffer = NULL;
int				Data::m_fileSize;	//0x00
Byte			*Data::m_data;		//0x20

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
		                ((Byte *) m_buffer) + totalBytesRead,		// start of data to write
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
	return true;
}

bool		Data::write(const std::string &name)
{
	if (m_buffer = NULL)
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
                       CREATE_NEW,             // create new file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);                  // no attr. template

    if (hFile == INVALID_HANDLE_VALUE) 
    { 
		std::cout << "Error: unable to open " << name << std::endl;
        return false;
    }


	for (int totalBytesWritten = 0; totalBytesWritten != m_fileSize; totalBytesWritten += bytesWritten)
	{
		bErrorFlag = WriteFile( 
		                hFile,           // open file handle
		                ((int *) m_buffer) + totalBytesWritten,      // start of data to write
		                m_fileSize - totalBytesWritten,  // number of bytes to write
		                &bytesWritten, // number of bytes that were written
		                NULL);            // no overlapped structure

	    if (FALSE == bErrorFlag)
	    {
			std::cout << "Error: unable to write on " << name << std::endl;
			CloseHandle(hFile);
			return false;
	    }
    }
	return true;
}

void		Data::solveEndianTo(int to)
{
	to = to / 4 + 8;
	for (int i = 8; i < to; i++)
		fixEndian(m_intBuffer[i]);
}