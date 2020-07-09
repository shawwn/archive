#include "LoggingInterface.h"
#include "Globals.h"
#include <iostream>
using namespace std;

LoggingInterface gLog;

LoggingInterface::LoggingInterface() : mUseCout(false)
{
	mFilename = "Default.log";
}

LoggingInterface::~LoggingInterface()
{
	Flush(true);
	*this << "Session end.\n";
}

void LoggingInterface::ClearFile()
{
	mOut.open(mFilename.c_str());

	*this << "Session start.\n";
}

void LoggingInterface::Flush(bool Force)
{
  std::string &StreamString = mStream.str();
	if(mUseCout)
		cout << StreamString;
	if(!mOut.is_open()) {
		if(Force) {
			ClearFile();
			if(!mOut.is_open()) {
				return;
			}
		}
		else
		{
			mPending.push_back(StreamString);
			mStream.str(string());
			return;
		}
	}
	while(mPending.begin() != mPending.end()) {
		mOut << *mPending.begin();
		mPending.erase(mPending.begin());
	}
	mOut << StreamString;
  //mStream.str(StreamString.substr(0,StreamString.length()-1));
  size_t newlinepos = StreamString.length()-1;
  if(StreamString[newlinepos] == '\n')
    StreamString.erase(newlinepos);
  if(gConsoleCallback)
    gConsoleCallback(StreamString.c_str());
	mStream.str(string());
}

LoggingInterface &LoggingInterface::Constructed(const std::string &Object)
{	
	if(gDebugSwitch) {
		*this << Object << ": Constructed.\n";
	}
	return *this;
}

LoggingInterface &LoggingInterface::Destructed(const std::string &Object)
{	
	if(gDebugSwitch) {
		*this << Object << ": Destructed.\n";
	}
	return *this;
}