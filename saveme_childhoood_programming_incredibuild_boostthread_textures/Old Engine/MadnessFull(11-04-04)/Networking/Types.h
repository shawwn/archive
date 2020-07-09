#ifndef _TYPES_H
#define _TYPES_H

#ifdef WIN32
typedef unsigned __int8 Byte;
typedef unsigned __int16 Word;
typedef unsigned __int32 DoubleWord;
typedef unsigned __int64 QuadWord;
typedef float Float;
#else
typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int DoubleWord;
typedef unsigned long long QuadWord;
typedef float Float;
#endif

#endif
