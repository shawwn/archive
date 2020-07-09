#ifndef LOGGINGINTERFACE_H
#define LOGGINGINTERFACE_H

#include <string>
#include <sstream>
#include <fstream>
#include <list>

class LoggingInterface
{
private:
	std::string			mFilename;
	std::ostringstream	mStream;
	std::ofstream		mOut;
	std::list<std::string> mPending;
	bool			mUseCout;


	template<class T>
		void Print(const T& t) {
			mStream << t;
			if(*mStream.str().rbegin() == '\n')
				Flush();
		}
	void Flush(bool Force = false);
	void ClearFile();
public:
	LoggingInterface();
	~LoggingInterface();
	
	template<class T>
		friend LoggingInterface &operator <<(LoggingInterface &l, const T &t) { l.Print(t); return l; }
	friend LoggingInterface &operator <<(LoggingInterface &l, const char *t) { l.Print(t); return l; }
	friend LoggingInterface &operator <<(LoggingInterface &l, const LoggingInterface &t) { return l; }

	inline void SetUseCout(bool Set) { mUseCout = Set; }
	inline void SetOutputFile(const std::string &Out) { mFilename = Out; ClearFile(); }

	LoggingInterface &Constructed(const std::string &Object);
	LoggingInterface &Destructed(const std::string &Object);
};
extern LoggingInterface gLog;

#endif