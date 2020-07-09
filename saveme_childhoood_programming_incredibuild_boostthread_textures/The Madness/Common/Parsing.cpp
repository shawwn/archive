#include "Parsing.h"
#include "LoggingInterface.h"
using namespace std;

unsigned long Hash(const std::string &str)
{
	unsigned long hash = 5381;
	for(size_t i = 0; i < str.length(); i++) 
		hash = ((hash << 5) + hash) + str[i];
	return hash;
}

std::string Trim(std::string Line)
{
	size_t Pos;
	if( (Pos = Line.find_first_of('#')) != std::string::npos)
		Line.erase(Pos);
	while(Line[0] == ' ' || Line[0] == 9 || Line[0] == '\n' || Line[0] == '\r')
		Line.erase(Line.begin(),Line.begin()+1);
	while(*Line.rbegin() == ' ' || *Line.rbegin() == 9 || *Line.rbegin() == '\n' || *Line.rbegin() == '\r')
		Line.erase(Line.length()-1);

	return Line;
}

std::string Upper(std::string Str)
{
	for(size_t i = 0; i < Str.length(); i++)
		Str[i] = toupper(Str[i]);
	return Str;
}

std::string Lower(std::string Str)
{
	for(size_t i = 0; i < Str.length(); i++)
		Str[i] = tolower(Str[i]);
	return Str;
}

void ParseToken(const std::string &Line, std::string &Variable, std::string &Value, char Delim)
{
	size_t Pos = Line.find_first_of(Delim);
	if(BADPOS(Pos)) {
		gLog << "WARNING ParseToken([" << Line << "],Variable,Value,[" << Delim << "]: No [" << Delim << "] found in line.\n";
		return;
	}
	Variable = Trim(Line.substr(0,Pos));
	Value = Trim(Line.substr(Pos+1,Line.length()));
}

void FirstToken(std::string &Line, std::string &Token, char Delim)
{
	size_t Pos = Line.find_first_of(Delim);
	if(BADPOS(Pos)) {
		Token = Line;
		Line = "";
	}
	else
	{
		Token = Line.substr(0,Pos);
		Line = Line.substr(Pos+1,Line.length());
	}
}

std::string FindPath(const std::string &Filename)
{
	if(FilenameIsValid(Filename)) return Filename;
	string Extention = Lower(GetExtention(Filename));
	string Try;
	if(Extention == "") return Filename;

	if(Extention == "bmp" || Extention == "jpg" || Extention == "jpeg" || Extention == "png" || Extention == "tga") {
		Try = "Data/Textures/" + Filename;
		if(FilenameIsValid(Try)) return Try;
		Try = "Data/Heightmaps/" + Filename;
		if(FilenameIsValid(Try)) return Try;
	}
	if(Extention == "dat") {
		Try = "Data/" + Filename;
		if(FilenameIsValid(Try)) return Try;
	}
	return Filename;
}