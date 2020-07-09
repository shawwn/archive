#ifndef PARSING_H
#define PARSING_H

#include <string>
#include <fstream>

#define ASCIITAB 9
#define PARSING_ALLOWWHITESPACE true

#define BADPOS( pos ) pos == std::string::npos

extern unsigned long Hash(const std::string &str);
extern std::string Trim(std::string Line);
extern std::string Upper(std::string Str);
extern std::string Lower(std::string Str);
extern void ParseToken(const std::string &Line, std::string &Variable, std::string &Value, char Delim = '=');
extern void FirstToken(std::string &Line, std::string &Token, char Delim = '/');
extern std::string FindPath(const std::string &Filename);

inline std::string GetExtention(const std::string &str) { size_t Pos = str.find_first_of('.'); return (BADPOS(Pos) ? "" : str.substr(Pos+1,str.length())); }
inline bool FilenameIsValid(const std::string &Filename) { std::fstream in(Filename.c_str()); return in.is_open(); }
inline bool IsNumber(char c) { return ((c-'0') < 10); }
inline bool IsNumerical(const std::string &str, bool AllowWhitespace = false) { 
	for(size_t i = 0; i < str.length(); i++) {
		if(!IsNumber(str[i])) {
			if(str[i] != ' ' && str[i] != ASCIITAB) 
				return false;
			if(!AllowWhitespace)
				return false;
		}
	}
	return true;
}
inline bool Contains(const std::string &str, char has) { return !BADPOS(str.find_first_of(has)); }


#endif