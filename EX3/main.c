#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC

#define READ 0
#define WRITE 1

#include "FileHandling.h"
#include "ProcessHandling.h"
#include <math.h>
#include <crtdbg.h>


typedef struct Frame
{
	int PageID;
	int valid;
	int finish_time;
}Frame;

typedef struct Page
{
	int frameID;
	int valid;
	int finishTime;
	int placementTime;
}Page;

typedef struct ThreadParameters
{
	int arrivalTime;
	int pageID;
	int timeOfUse;
	long maxFrame;
}Params;

DWORD WINAPI threadExecute(int arrivalTime, int pageID, int timeOfUse, long maxFrame)
{
	printf("%d, %d, %d, %ld\n", arrivalTime, pageID, timeOfUse, maxFrame);
	return 0;
}

/// <summary>
/// This function gets a line and extracts the
/// </summary>
/// <param name="data"></param>
/// <param name="time"></param>
/// <param name="virtualBits"></param>
/// <param name="physicalBits"></param>
void ParseData(char* data, int* time, int* virtualAddress, int* timeOfUse)
{
	const char delim[] = " ";
	*time = atoi(strtok_s(data, delim, &data));
	*virtualAddress = atoi(strtok_s(data, delim, &data)) >> 12;
	*timeOfUse = atoi(strtok_s(data, delim, &data));
}

int main(int argc, char* argv[])
{
	int a = (atoi(argv[1]) - 12), b = (atoi(argv[2]) - 12);
	long maxPage = pow(2, a);
	long maxFrame = pow(2, b);

	// open input file
	HANDLE inputFile = NULL;
	if (openFile(&inputFile, argv[3], READ))
	{
		//first file failed
		return 1;
	}

	// read data from file
	char* data = NULL;
	int size = GetFileSize(inputFile, NULL);
	if (size == INVALID_FILE_SIZE)
	{
		// GetFileSize failed
		return 1;
	}
	data = malloc(sizeof(char*) * size);
	if (data == NULL)
	{
		// malloc failed
		printf("error allocating memory\n");
		return 1;
	}
	if (ReadFromFile(inputFile, data, size))
	{
		// read failed
		printf("Error reading from file\n");
		return 1;
	}
	
	char* temp = data;

	const char delim[] = "\n";
	Params p;
	int arrivalTime = 0, pageID = 0, timeOfUse = 0;

	char* RealFileData = strtok_s(data, delim, &data);
	
	ParseData(RealFileData, &arrivalTime, &pageID, &timeOfUse);
	
	p.arrivalTime = arrivalTime;
	p.maxFrame = maxFrame;
	p.pageID = pageID;
	p.timeOfUse = timeOfUse;
	
	HANDLE threadh = NULL;
	DWORD threadIDs[10];

	if (openThread(&threadh, &threadExecute, &p, &threadIDs[0]))
	{
		printf("error opening Thread \n");	//Thread won't open, Inform the User and Continue
		free(temp); 
		return 1;
	}
	WaitForSingleObject(threadh, 10000000000);

	free(temp);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	printf("%d\n", _CrtDumpMemoryLeaks()); // check for memory leaks
	return 0;
}