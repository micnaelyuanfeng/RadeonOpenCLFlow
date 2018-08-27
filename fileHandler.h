#pragma once

#ifndef _FILE_HANDLER_
#define _FILE_HANDLER_

//using namespace std; //need to add this to pass compile

class fileHandler
{
public:
	fileHandler();
	~fileHandler();

	bool writeBinaryToFile(const uint8* fileName, const uint8* binary, size_t numOfBytes);
	bool readBinaryFromFile(const uint8* fileName);
	bool open(const uint8* fileName);
	string& source(void) { return sourceFile; }
private:
	string sourceFile;
};

#endif