
#include "dxstdafx.h"
#include "LoggingInterface.h"
#include "Engine.h"
#include "ParsingInterface.h"


LoggingInterface *gLog;

LoggingInterface::LoggingInterface()
{
}

LoggingInterface::~LoggingInterface()
{
	Destroy();
}

void LoggingInterface::CheckOpen()
{
	if(!Out.is_open())
	{
		Out.open("DefaultLog.log");
		Out << "There is no active log file, using DefaultLog.log\n";
	}
}

void LoggingInterface::ClearStream(ostringstream *stream)
{
	if(!this)
		return;
	if(stream)
		stream->str(std::string());
}

void LoggingInterface::SetOutputFile(const char *Path)
{
	Out.close();
	Out.clear();
	Out.open(Path);
	if(Out.is_open())
	{
		OutputFile = Path;
	}
	else
	{
		Out.open(OutputFile.c_str(),ios::app);
		CheckOpen();
	}	
}

void LoggingInterface::Print()
{
	if(!this)
		return;
	CheckOpen();

	Out << Stream.str() << "\n";

	wstring str = StringToWideString(Stream.str().c_str());
	gEngine->Print(str);

	ClearStream(&Stream);
}

void LoggingInterface::Print(const char *Message)
{
	if(!this)
		return;
	CheckOpen();
	Out << Message << endl;

	wstring str = StringToWideString(Message);
	gEngine->Print(str);
}

void LoggingInterface::Destroy()
{
	Out.close();
	Out.clear();
}
