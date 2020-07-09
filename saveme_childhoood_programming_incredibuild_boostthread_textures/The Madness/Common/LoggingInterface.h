#ifndef LOGGINGINTERFACE_H
#define LOGGINGINTERFACE_H

#include <string>
#include <fstream>
#include <sstream>
#include "Types.h"

class LoggingInterface
{
private:
	std::string mOutputFile;
	std::ofstream mOut;
	std::ostringstream mStream;
	bool mUseCout;
	bool mDisable;

	void ResetFile();

	template< typename T >
	void Print(T item);

	template< typename T >
	void PrintFatal(T item);
public:
	LoggingInterface() : mOutputFile("Default.log"), mUseCout(false), mDisable(false) { }
	~LoggingInterface() { mOut.close(); mOut.clear(); }

	inline void SetOutputFile(const std::string &Filename) { mOutputFile = Filename; ResetFile(); }
	inline void SetUseCout(bool Set) { mUseCout = Set; }
	inline void SetDisable(bool Set) { mDisable = Set; }
	inline std::ostringstream &GetStream() { return mStream; }

	/* Debug operator '%', use gLog % "Debug Info", and unless #ifdef LOGDEBUGINFO == true, it won't be logged */
	friend LoggingInterface & operator % (LoggingInterface &l, const std::string &str);
	friend LoggingInterface & operator % (LoggingInterface &l, const char *str);
	friend LoggingInterface & operator % (LoggingInterface &l, __int8  c);
	friend LoggingInterface & operator % (LoggingInterface &l, __int16 s);
	friend LoggingInterface & operator % (LoggingInterface &l, __int32 i);
	friend LoggingInterface & operator % (LoggingInterface &l, float f);
	friend LoggingInterface & operator % (LoggingInterface &l, double d);
	friend LoggingInterface & operator % (LoggingInterface &l, Byte b);
	friend LoggingInterface & operator % (LoggingInterface &l, Word w);
	friend LoggingInterface & operator % (LoggingInterface &l, DoubleWord d);
	friend LoggingInterface & operator % (LoggingInterface &l, QuadWord q);

	friend LoggingInterface & operator << (LoggingInterface &l, const std::string &str);
	friend LoggingInterface & operator << (LoggingInterface &l, const char *str);
	friend LoggingInterface & operator << (LoggingInterface &l, __int8  c);
	friend LoggingInterface & operator << (LoggingInterface &l, __int16 s);
	friend LoggingInterface & operator << (LoggingInterface &l, __int32 i);
	friend LoggingInterface & operator << (LoggingInterface &l, float f);
	friend LoggingInterface & operator << (LoggingInterface &l, double d);
	friend LoggingInterface & operator << (LoggingInterface &l, Byte b);
	friend LoggingInterface & operator << (LoggingInterface &l, Word w);
	friend LoggingInterface & operator << (LoggingInterface &l, DoubleWord d);
	friend LoggingInterface & operator << (LoggingInterface &l, QuadWord q);

	/* Fatal operator '^', use gLog ^ "Ending info" and it will end the program with an exception */
	friend LoggingInterface & operator ^(LoggingInterface &l, const std::string &str);
	friend LoggingInterface & operator ^(LoggingInterface &l, const char *str);
	friend LoggingInterface & operator ^ (LoggingInterface &l, __int8  c);
	friend LoggingInterface & operator ^ (LoggingInterface &l, __int16 s);
	friend LoggingInterface & operator ^ (LoggingInterface &l, __int32 i);
	friend LoggingInterface & operator ^ (LoggingInterface &l, float f);
	friend LoggingInterface & operator ^ (LoggingInterface &l, double d);
	friend LoggingInterface & operator ^ (LoggingInterface &l, Byte b);
	friend LoggingInterface & operator ^ (LoggingInterface &l, Word w);
	friend LoggingInterface & operator ^ (LoggingInterface &l, DoubleWord d);
	friend LoggingInterface & operator ^ (LoggingInterface &l, QuadWord q);

};

extern LoggingInterface gLog;

#define WARN( x , y ) gLog << #x << " WARNING: " << ##y << "\n";
#define ERR( x , y ) gLog << #x << " ERROR: " << ##y << "\n";

#endif