#include "FileHandling.h"
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>

/// <summary>
/// This function creates a file using winAPI's CreateFileA function.
///
/// ASSUMPTIONS: 
///		access mode is either GENERIC_WRITE or GENERIC_READ.
///		share mode is 0 (no share).
///		creation disposition is either OPEN_ALWAYS or OPEN_EXISTING, depending on the access mode.
/// </summary>
/// <param name="hfile"> - a HANDLE* object to which the function will place the created file handle. should be initiated to NULL.</param>
/// <param name="fileName"> - a char* object containing the file's name.</param>
/// <param name="accessMode"> - desired access mode to the file. 0-read; 1-write</param>
/// <returns>returns 0 if secceeded, 1 otherwise</returns>
int openFile(HANDLE *hfile, char *fileName, int accessMode)
{
	*hfile = CreateFileA(fileName, (accessMode)? GENERIC_WRITE:GENERIC_READ, 0, NULL, (accessMode)? OPEN_ALWAYS:OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (*hfile == INVALID_HANDLE_VALUE)
	{
		printf("Error occured during the creation/opening of %s\n", fileName);
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
		return 1;
	}
	return 0;
}

/// <summary>
/// This function writes data to a file using winAPI's WriteFile function. Failure can be caused by WriteFile failure, or when the written data's length isn't equal to the given messageLen.
/// </summary>
/// <param name="hfile"> - a HANDLE object to the file that is written to.</param>
/// <param name="buffer"> - a char* object containing the data that will be written.</param>
/// <param name="messageLen"> - an int containing the message's length. (should be equal to strlen(buffer) after the read was done).</param>
/// <returns> The function returns 0 if succeeded, 1 otherwise.</returns>
int WriteToFile(HANDLE hfile, char* buffer, int messageLen)
{
	DWORD writtenMassageLen;
	if (!WriteFile(hfile, buffer, messageLen, &writtenMassageLen, NULL))
	{
		/*printf("Error occured during the writing to file \n");
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));*/
		return 1;
	}
	if (writtenMassageLen != messageLen)
	{
		printf("Error occured during the writing to the file \n");
		printf("Not all of the data was written successfully\n");
		return 1;
	}
	return 0;
}

/// <summary>
/// This function reads data from a file and places it to a buffer, using winAPI's ReadFile function. Failure can be caused by ReadFile failure, or when the read data's length isn't equal to the given messageLen.
/// </summary>
/// <param name="hfile"> - a HANDLE object to the file that is read.</param>
/// <param name="buffer"> - a char* object containing the data that will be written.</param>
/// <param name="messageLen"> - an int containing the message's length. (should be equal to strlen(buffer)).</param>
/// <returns> The function returns 0 if succeeded, 1 otherwise.</returns>
int ReadFromFile(HANDLE hfile, char* buffer, int messageLen)
{
	DWORD readMassageLen;
	if (!ReadFile(hfile, buffer, messageLen, &readMassageLen, NULL))
	{
		/*printf("Error occured during the reading from file \n");
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));*/
		return 1;
	}
	buffer[messageLen] = '\0';

	if (readMassageLen != messageLen)
	{
		/*printf("Error occured during the reading from the file \n");
		wchar_t buf[256];
		printf("Not all of the data was read successfully\n");*/
		return 1;
	}
	return 0;
}

/// <summary>
/// This function receives a pointer to a handle and closes it, using winAPI's CloseHandle function. if an error occured, prints the GetLastError() value.
/// </summary>
/// <param name="hfile"> - pointer to a handle.</param>
/// <returns> returns 0 if secceeded, 1 otherwise. </returns>
int closeFile(HANDLE* hfile)
{
	if (!CloseHandle(*hfile))
	{
		//printf("Error occured during the closing file\n");
		//wchar_t buf[256];
		//printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		//NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		//buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
		return 1;
	}
	return 0;
}

/// <summary>
/// This function receives a pointer to a handle and an int, then moves the file pointer with the givven offset using winAPI's SetFilePointer function. if an error occured, prints the GetLastError() value.
/// </summary>
/// <param name="hfile"> - a HANDLE* to the file.</param>
/// <param name="offset"> - the offset given to the file pointer.</param>
/// <returns>Returns 0 if succeeded, 1 otherwise.</returns>
/// 

/// <summary>
/// This function receives a pointer to a handle and an int, then moves the file pointer with the givven offset using winAPI's SetFilePointer function. if an error occured, prints the GetLastError() value.
/// </summary>
/// <param name="hfile" - a HANDLE* to the file.</param>></param>
/// <param name="offset"> - the offset given to the file pointer.</param></param>
/// <param name="accessMode"> - desired access mode to the file. 0-read; 1-write</param>
/// <returns></returns>
int MoveFilePointer(HANDLE* hfile, int offset, int accessMode, char* filename)
{
	if (SetFilePointer(*hfile, (long)offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		printf("Error occured during the moving file pointer: %s\n", filename);
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
		return 1;
	}
	return 0;
}

int createDir(char* pathName)
{
	if (!CreateDirectoryA(pathName, NULL))
	{
		/*printf("Error occured during the creating directory: %s\n", pathName);
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));*/
		return 1;
	}
	return 0;
}