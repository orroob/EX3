#pragma once

#ifndef HEADER_FILE
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
int openFile(HANDLE* hfile, char* fileName, int accessMode);

/// <summary>
/// This function writes data to a file using winAPI's WriteFile function. Failure can be caused by WriteFile failure, or when the written data's length isn't equal to the given messageLen.
/// </summary>
/// <param name="hfile"> - a HANDLE object to be written.</param>
/// <param name="buffer"> - a char* object containing the data that will be written.</param>
/// <param name="messageLen"> - an int containing the message's length. (should be equal to strlen(buffer)).</param>
/// <returns> The function returns 0 if succeeded, 1 otherwise.</returns>
int WriteToFile(HANDLE hfile, char* buffer, int messageLen);

/// <summary>
/// This function reads data from a file and places it to a buffer, using winAPI's ReadFile function. Failure can be caused by ReadFile failure, or when the read data's length isn't equal to the given messageLen.
/// </summary>
/// <param name="hfile"> - a HANDLE object to the file that is read.</param>
/// <param name="buffer"> - a char* object containing the data that will be written.</param>
/// <param name="messageLen"> - an int containing the message's length. (should be equal to strlen(buffer)).</param>
/// <returns> The function returns 0 if succeeded, 1 otherwise.</returns>
int ReadFromFile(HANDLE hfile, char* buffer, int len);

/// <summary>
/// This function receives a pointer to a handle and closes it. if an error occured, prints the GetLastError() value.
/// </summary>
/// <param name="hfile"> - pointer to a handle.</param>
/// <returns> returns 0 if secceeded, 1 otherwise. </returns>
int closeFile(HANDLE* hfile);


int MoveFilePointer(HANDLE* hfile, int offset, int accessMode, char* filename);


int createDir(char* pathName);

#endif