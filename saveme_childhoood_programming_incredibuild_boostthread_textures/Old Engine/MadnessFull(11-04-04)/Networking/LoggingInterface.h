#ifndef _LOGGINGINTERFACE_H
#define _LOGGINGINTERFACE_H

#include <fstream>
#include <string>
#include <sstream>
#include "Bsd.h"
using namespace std;

class LoggingInterface
{
private:
	string OutputFile;
	ofstream Out;

	void CheckOpen();
public:
	LoggingInterface();
	~LoggingInterface();
	void SetOutputFile(const char *Path);
	void Print();
	void Print(const char *Message);
	void ClearStream(ostringstream *stream);
	void Destroy();

	ostringstream Stream;
};

extern LoggingInterface *gLog;

#endif
