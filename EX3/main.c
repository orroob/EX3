#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC

#define READ 0
#define WRITE 1

#include "FileHandling.h"
#include "ProcessHandling.h"
#include <math.h>
#include <crtdbg.h>
#include <string.h>


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


Frame* FramesArr;
HANDLE GlobalSemaphore;

void updateFrame(int frameID, int pageID, int finishTime)
{
	FramesArr[frameID].finish_time = finishTime;
	FramesArr[frameID].PageID = pageID;
	FramesArr[frameID].valid = 1;
}

DWORD WINAPI threadExecute(Params *parameters)
{
	int arrivalTime = parameters->arrivalTime, pageID = parameters->pageID, timeOfUse = parameters->timeOfUse, maxFrame = parameters->maxFrame;
	//printf("%d, %d, %d, %ld\n", arrivalTime, pageID, timeOfUse, maxFrame);
	for (int i = 0; i < maxFrame; i++)
	{
		if (FramesArr[i].PageID == pageID)
		{
			updateFrame(i, pageID, arrivalTime + timeOfUse);
			return 0;
		}
	}

	WaitForSingleObject(GlobalSemaphore, INFINITE);

	for (int i = 0; i < maxFrame; i++)
	{
		if (FramesArr[i].valid == 0)
		{
			updateFrame(i, pageID, arrivalTime + timeOfUse);
			break;
		}
	}

	if (!ReleaseSemaphore(GlobalSemaphore, 1, NULL))
	{
		printf("problem\n");
		return 1;
	}

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
	long maxPage = pow(2, a), maxFrame = pow(2, b);

	// allocate memory for frames array
	if ((FramesArr = malloc(maxFrame * sizeof(Frame))) == NULL)
	{
		printf("error allocating memory\n");
		return 1;
	}
	for (int i = 0; i < maxFrame; i++)
	{
		FramesArr[i].valid = 0;
	}

	// create semaphore object
	if (openSemaphore(&GlobalSemaphore, maxFrame, maxFrame, "sema1"))
	{
		return 1;
	}

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
	Params *p[10];
	p[0] = malloc(10*sizeof(Params*));
	p[1] = malloc(10 * sizeof(Params*));
	int arrivalTime = 0, pageID = 0, timeOfUse = 0;

	char* line = strtok_s(data, delim, &data);

	line = strtok_s(data, delim, &data);

	ParseData(line, &arrivalTime, &pageID, &timeOfUse);
	
	p[0]->arrivalTime = arrivalTime;
	p[0]->maxFrame = maxFrame;
	p[0]->pageID= pageID;
	p[0]->timeOfUse= timeOfUse;

	line = strtok_s(data, delim, &data);
	line = strtok_s(data, delim, &data);

	ParseData(line, &arrivalTime, &pageID, &timeOfUse);

	p[1]->arrivalTime = arrivalTime;
	p[1]->maxFrame = maxFrame;
	p[1]->pageID = pageID;
	p[1]->timeOfUse = timeOfUse;
	
	HANDLE threadh = NULL, threadh2 = NULL, threadh3 = NULL;
	DWORD threadIDs[10];

	if (openThread(&threadh, &threadExecute, p[0], &threadIDs[0]))
	{
		printf("error opening Thread \n");	//Thread won't open, Inform the User and Continue
		free(temp); 
		return 1;
	}
	if (openThread(&threadh2, &threadExecute, p[1], &threadIDs[0]))
	{
		printf("error opening Thread \n");	//Thread won't open, Inform the User and Continue
		free(temp);
		return 1;
	}
	
	WaitForSingleObject(threadh2, INFINITE);

	free(FramesArr);
	free(p[0]);
	free(p[1]);
	free(temp);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	printf("%d\n", _CrtDumpMemoryLeaks()); // check for memory leaks
	return 0;
}