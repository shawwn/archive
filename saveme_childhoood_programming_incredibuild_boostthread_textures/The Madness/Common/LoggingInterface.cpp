#include "LoggingInterface.h"
#include <iostream>
using namespace std;

//#ifdef _DEBUG
//#define LOGDEBUGINFO
//#endif
		

LoggingInterface gLog;

void LoggingInterface::ResetFile()
{
	mOut.open(mOutputFile.c_str());
	mOut.close();
	mOut.open(mOutputFile.c_str(),ios::app);
}

template<typename T>
void LoggingInterface::Print(T item)
{
	if(mDisable)
		return;
	if(!mOut.is_open())
		ResetFile();
	mStream << item;
	if(*mStream.str().rbegin() == '\n') {
		if(mUseCout) { cout << mStream.str(); }
		mOut << mStream.str();
		mOut.flush();
		mStream.str(std::string());
	}
}

template<typename T>
void LoggingInterface::PrintFatal(T item)
{
	if(mDisable)
		return;
	if(!mOut.is_open())
		ResetFile();
	mStream << item;
	if(*mStream.str().rbegin() == '\n') {
		if(mUseCout) { cout << mStream.str(); }
		mOut << mStream.str();
#ifdef _DEBUG
		throw mStream.str();
#endif
	}
	mStream.str(std::string());
}

LoggingInterface & operator % (LoggingInterface &l, const string &str)
{
#ifdef LOGDEBUGINFO
	l.Print(str);
#endif
	return l;
}

LoggingInterface & operator % (LoggingInterface &l, const char *str)
{
#ifdef LOGDEBUGINFO
	l.Print(str);
#endif
	return l;
}
LoggingInterface & operator % (LoggingInterface &l, __int8  c)
{
#ifdef LOGDEBUGINFO
	l.Print(c);
#endif
	return l;
}
LoggingInterface & operator % (LoggingInterface &l, __int16 s)
{
#ifdef LOGDEBUGINFO
	l.Print(s);
#endif
	return l;
}
LoggingInterface & operator % (LoggingInterface &l, __int32 i)
{
#ifdef LOGDEBUGINFO
	l.Print(i);
#endif
	return l;
}

LoggingInterface & operator % (LoggingInterface &l, float f)
{
#ifdef LOGDEBUGINFO
	l.Print(f);
#endif
	return l;
}
LoggingInterface & operator % (LoggingInterface &l, double d)
{
#ifdef LOGDEBUGINFO
	l.Print(d);
#endif
	return l;
}

LoggingInterface & operator % (LoggingInterface &l, Byte b)
{
#ifdef LOGDEBUGINFO
	l.Print(b);
#endif
	return l;
}
LoggingInterface & operator % (LoggingInterface &l, Word w)
{
#ifdef LOGDEBUGINFO
	l.Print(w);
#endif
	return l;
}
LoggingInterface & operator % (LoggingInterface &l, DoubleWord d)
{
#ifdef LOGDEBUGINFO
	l.Print(d);
#endif
	return l;
}
LoggingInterface & operator % (LoggingInterface &l, QuadWord q)
{
#ifdef LOGDEBUGINFO
	l.Print(q);
#endif
	return l;
}

LoggingInterface & operator << (LoggingInterface &l, const string &str)
{
	l.Print(str);
	return l;
}

LoggingInterface & operator << (LoggingInterface &l, const char *str)
{
	l.Print(str);
	return l;
}

LoggingInterface & operator << (LoggingInterface &l, __int8  c)
{
	l.Print(c);
	return l;
}
LoggingInterface & operator << (LoggingInterface &l, __int16 s)
{
	l.Print(s);
	return l;
}
LoggingInterface & operator << (LoggingInterface &l, __int32 i)
{
	l.Print(i);
	return l;
}

LoggingInterface & operator << (LoggingInterface &l, float f)
{
	l.Print(f);
	return l;
}
LoggingInterface & operator << (LoggingInterface &l, double d)
{
	l.Print(d);
	return l;
}

LoggingInterface & operator << (LoggingInterface &l, Byte b)
{
	l.Print(b);
	return l;
}
LoggingInterface & operator << (LoggingInterface &l, Word w)
{
	l.Print(w);
	return l;
}
LoggingInterface & operator << (LoggingInterface &l, DoubleWord d)
{
	l.Print(d);
	return l;
}
LoggingInterface & operator << (LoggingInterface &l, QuadWord q)
{
	l.Print(q);
	return l;
}

LoggingInterface & operator ^ (LoggingInterface &l, const string &str)
{
	l.PrintFatal(str);
	return l;
}

LoggingInterface & operator ^ (LoggingInterface &l, const char *str)
{
	l.PrintFatal(str);
	return l;
}

LoggingInterface & operator ^ (LoggingInterface &l, __int8  c)
{
	l.PrintFatal(c);
	return l;
}
LoggingInterface & operator ^ (LoggingInterface &l, __int16 s)
{
	l.PrintFatal(s);
	return l;
}
LoggingInterface & operator ^ (LoggingInterface &l, __int32 i)
{
	l.PrintFatal(i);
	return l;
}

LoggingInterface & operator ^ (LoggingInterface &l, float f)
{
	l.PrintFatal(f);
	return l;
}
LoggingInterface & operator ^ (LoggingInterface &l, double d)
{
	l.PrintFatal(d);
	return l;
}

LoggingInterface & operator ^ (LoggingInterface &l, Byte b)
{
	l.PrintFatal(b);
	return l;
}
LoggingInterface & operator ^ (LoggingInterface &l, Word w)
{
	l.PrintFatal(w);
	return l;
}
LoggingInterface & operator ^ (LoggingInterface &l, DoubleWord d)
{
	l.PrintFatal(d);
	return l;
}
LoggingInterface & operator ^ (LoggingInterface &l, QuadWord q)
{
	l.PrintFatal(q);
	return l;
}