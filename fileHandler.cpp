#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "types.h"
#include "fileHandler.h"

fileHandler::fileHandler()
{

}

fileHandler::~fileHandler()
{

}

bool fileHandler::writeBinaryToFile(const uint8* fileName, const uint8* binary, size_t numOfBytes)
{
	FILE* output = NULL;

	bool  result = true;

	result = fopen_s(&output, (char*)fileName, "wb");

	if (result == NULL)
	{
		cout << "Error: Writing Binary to File." << endl;

		result = false;
	}

	fwrite(binary, sizeof(char), numOfBytes, output);

	fclose(output);

	return result;
}

bool fileHandler::readBinaryFromFile(const uint8* fileName)
{
	FILE* input  = NULL;
	char* binary = NULL;
	size_t size  = 0;

	bool result  = true;

	result = fopen_s(&input, (char*)fileName, "rb");

	if (result == NULL)
	{
		cout << "Error: Read binary from File." << endl;

		result = false;
	}

	fseek(input, 0, SEEK_END);

	size = ftell(input);

	binary = (char*)new char[size];

	if (binary == NULL)
	{
		cout << "Error: Create memory for Binary reading." << endl;

		result = false;
	}

	fread(binary, sizeof(char), size, input);

	sourceFile.assign(binary, size);

	fclose(input);

	delete[] binary;

	return result;
}

bool fileHandler::open(const uint8* fileName)
{
	size_t size;
	uint8* sourceFileTemp;
	bool   result = true;

	fstream thisFileHandler((char*)fileName, fstream::in | fstream::binary);

	if (thisFileHandler.is_open())
	{
		size_t fileSize;

		thisFileHandler.seekg(0, fstream::end);

		size = fileSize = (size_t)thisFileHandler.tellg();

		thisFileHandler.seekg(0, fstream::beg);

		sourceFileTemp = new uint8[fileSize + 1];

		if (sourceFileTemp == NULL)
		{
			cout << "Error: Creating File Memory Space." << endl;

			thisFileHandler.close();

			result = false;
		}
		else
		{
			thisFileHandler.read((char*)sourceFileTemp, fileSize);
		
			thisFileHandler.close();

			sourceFileTemp[size] = '\0';

			sourceFile = (char*)sourceFileTemp; //operator = overloaded for string

			delete[] sourceFileTemp;
		}
	}

	return result;
}