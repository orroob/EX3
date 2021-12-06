#include "ProcessHandling.h"

int openProcess(char* command, STARTUPINFO	*startinfo, PROCESS_INFORMATION *procinfo)
{
	
	if (!CreateProcessA(NULL, command, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, startinfo, procinfo))
	{
		printf("Error occured during the creation of process\n");
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
		return 1;
	}
	return 0;
}

/// <summary>
///This function creates a Thread using winAPI's CreateThread function.
/// </summary>
/// <param name="threadHandle">a HANDLE* object to which the function will place the created Thread handle</param>
/// <param name="function">A pointer to the application-defined function to be executed by the thread.</param>
/// <param name="parameters">A pointer to a variable to be passed to the thread.</param>
/// <param name="threadID">A pointer to a variable that receives the thread identifier</param>
/// <returns></returns>
int openThread(HANDLE *threadHandle, LPTHREAD_START_ROUTINE function, VOID *parameters, LPDWORD *threadID)
{
	*threadHandle = CreateThread(NULL, 0, function, parameters, 0, threadID);
	if (threadHandle == NULL)
	{
		/*printf("Error occured during the creation of thread\n");
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));*/
		return 1;
	}
	return 0;
}

int closeProcess(HANDLE *hProcess)
{
	if (!CloseHandle(*hProcess))
	{
		printf("Error occured during the closing process\n");
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
		return 1;
	}
	return 0;
}

int openSemaphore(HANDLE *semaphoreh, long initCount, long maxCount, char* name)
{
	*semaphoreh = CreateSemaphoreA(NULL, initCount, maxCount, name);
	if (semaphoreh == NULL)
	{
		printf("Error occured during creating semaphore\n");
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
		return 1;
	}
	return 0;
}