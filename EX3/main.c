#include "FileHandling.h"
#include "ProcessHandling.h"
#include <math.h>

#define READ 0
#define WRITE 1

typedef struct Frame
{
	int PageID;
	int valid;
	int finish_time;
};

typedef struct Page
{
	int frameID;
	int valid;
	int finish_time;
}Page;

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
	*virtualAddress = atoi(strtok_s(data, delim, &data));
	*timeOfUse = atoi(strtok_s(data, delim, &data));
}

int main(int argc, char* argv[])
{
	int a = (atoi(argv[1]) - 12), b = (atoi(argv[2]) - 12);
	long maxPage = pow(2, a);
	long maxFrame = pow(2, b);

	// open input file
	HANDLE inputFile = NULL;
	if (openFile(inputFile, argv[3], READ))
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
	if (readFileSimple(inputFile, data, size))
	{
		// read failed
		printf("Error reading from file\n");
		return 1;
	}
	
	char* temp = data;




	free(temp);
}